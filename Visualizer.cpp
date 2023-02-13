#include "Visualizer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

#define MOUSE_X (this->sfEvent.mouseButton.x)
#define MOUSE_Y (this->sfEvent.mouseButton.y)
#define MOUSE_HOVER_X ((float)sf::Mouse::getPosition(*this->window).x)
#define MOUSE_HOVER_Y ((float)sf::Mouse::getPosition(*this->window).y)

// Use these only for drawing shapes (shapes drawn to wrong part of cursor)
#define MOUSE_X_CORRECTED (this->sfEvent.mouseButton.x-30)
#define MOUSE_Y_CORRECTED (this->sfEvent.mouseButton.y-30)
#define MOUSE_HOVER_X_CORRECTED ((float)sf::Mouse::getPosition(*this->window).x-30)
#define MOUSE_HOVER_Y_CORRECTED ((float)sf::Mouse::getPosition(*this->window).y-30)


#ifndef CHECK_THREAD_AND_JOIN
#define CHECK_THREAD_AND_JOIN \
if (is_finished) {            \
    algo_thread.join();       \
    is_finished = false;      \
}
#endif

std::mutex run_lock; // prevents multiple threads from executing runBfs method at the same time
std::mutex window_lock; // prevents multiple threads from accessing window resource
bool algo_thread_is_running = false; // true when a thread is currently running runBfs
bool is_finished = false; // true when a thread ran and ended execution of runBfs, so we need to call join()
bool should_end = false;

bool is_immediate = false;

std::thread algo_thread; // the thread we use to execute runBfs




Visualizer::Visualizer(const Graph& graph): graph(graph) {
    this->window = new sf::RenderWindow(sf::VideoMode(1400, 1000), "Graph Visualizer");
    this->window->setFramerateLimit(60);
}



Visualizer::~Visualizer() {
  delete this->window;
}



void Visualizer::run() {
    while (this->window->isOpen()) {
        this->update();
        if (!algo_thread_is_running) {
            this->render();
        }
    }
}



void Visualizer::update() {
    CHECK_THREAD_AND_JOIN;
    while(this->window->pollEvent(this->sfEvent)) {
        CHECK_THREAD_AND_JOIN;
        switch (this->sfEvent.type) {
            case sf::Event::Closed: {
                should_end = true;
                if (algo_thread_is_running || is_finished)
                    algo_thread.join();
                this->window->close();
                break;
            }
            case sf::Event::MouseButtonPressed: {
                executeClickAction();
                break;
            }
            case sf::Event::Resized: {
                window_lock.lock();
                window->setActive(true);
                sf::FloatRect view(0, 0, (float)this->sfEvent.size.width, (float)this->sfEvent.size.height);
                this->window->setView(sf::View(view));
                window->setActive(false);
                window_lock.unlock();
                break;
            }
            default: {
                break;
            }
        }
    }
}





void Visualizer::render() {
    window_lock.lock();
    window->setActive(true);
    this->window->clear(BG_COLOR);
    this->toolbar.render(*this->window);
    this->graph.render(*this->window);
    if (this->toolbar.getActiveButtonId() == ADD_NODE) {
        sf::CircleShape hover_node(30);
        hover_node.setFillColor(sf::Color(255, 255, 255, 100));
        hover_node.setPosition(MOUSE_HOVER_X_CORRECTED, MOUSE_HOVER_Y_CORRECTED);
        this->window->draw(hover_node);
    }
    if (this->toolbar.getActiveButtonId() == ADD_EDGE && this->node_is_clicked) {
        std::shared_ptr<Node> tmp_node = std::make_shared<Node>(Node("tmp", MOUSE_HOVER_X_CORRECTED, MOUSE_HOVER_Y_CORRECTED));
        std::shared_ptr<Edge> tmp_edge = std::make_shared<Edge>(Edge(this->clicked_node, tmp_node, false));
        tmp_edge->render(*this->window);
    }
    this->window->display();
    window->setActive(false);
    window_lock.unlock();
}




void Visualizer::executeClickAction() {
    is_immediate = this->toolbar.updateActiveButton(sf::Vector2i(MOUSE_X, MOUSE_Y));
    ButtonId id = this->toolbar.getActiveButtonId();

    switch (id) {
        case RUN_BFS: {
            runBFSRoutine();
            break;
        }
        case RUN_DFS: {
            runDfSRoutine();
            break;
        }
        case RUN_DIJKSTRA: {
            if (algo_thread_is_running || !is_immediate) break;
            sf::Vector2i original_active_button_coordinates = sf::Vector2i(MOUSE_X, MOUSE_Y);
            while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
                this->window->pollEvent(this->sfEvent);
            }
            if (!this->toolbar.updateActiveButton(sf::Vector2i(MOUSE_X, MOUSE_Y)) || this->toolbar.getActiveButtonId() != RUN_DIJKSTRA) {
                this->toolbar.updateActiveButton(original_active_button_coordinates);
                break;
            }
            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
                this->graph.reset();
            }
            this->mode = DIJKSTRA;
            runAlgorithm();
            break;
        }
        case END: {
            endRoutine();
            break;
        }
        case RESET: {
            resetRoutine();
            break;
        }
        case CLEAR_WINDOW: {
            clearWindowRoutine();
            break;
        }
        default: {
            break;
        }

    }

    if (is_immediate || algo_thread_is_running) return;
    // the following cases are not allowed while an algorithm is running
    std::shared_ptr<Node> to_toggle = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
    this->graph.setToggledNode(to_toggle);
    switch (id) {
        case CURSOR: {
            cursorRoutine();
            break;
        }
        case ADD_NODE: {
            addNodeRoutine();
            break;
        }
        case ADD_EDGE: {
            addEdgeRoutine();
            break;
        }
        case ERASE: {
            eraseRoutine();
            break;
        }
        case CHANGE_START_NODE: {
            changeStartNodeRoutine();
            break;
        }
        case CHANGE_EDGE_WEIGHT: {
            std::shared_ptr<Edge> edge = this->graph.getEdgeByPosition(MOUSE_X, MOUSE_Y);
            if (!edge) break;
            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE)
                this->graph.reset();
            int weight = 5;
            edge->setState(EDGE_SELECTED);
            edge->setWeight(weight);
            break;
        }
        default: {
            break;
        }
    }

}

void Visualizer::runAlgorithm() {
    if (!this->graph.getStartNode() || algo_thread_is_running) return;
    VisMode current_mode = this->mode;
    bool should_wait = true;
    switch (current_mode) {
        case BFS: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runBFS, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), should_wait);
            break;
        }
        case DFS: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runDFS, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), should_wait);
            break;
        }
        case MST: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runMST, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), should_wait);
            break;
        }
        case DIJKSTRA: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runDijkstra, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), should_wait);
            break;
        }
    }
}




void Visualizer::cursorRoutine() {

}


void Visualizer::addNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.addNode(MOUSE_X_CORRECTED, MOUSE_Y_CORRECTED);
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists) {
        this->graph.reset();
    }
}


void Visualizer::addEdgeRoutine() {
    bool edge_was_added = false;
    std::shared_ptr<Node> dst = nullptr;
    if (!this->node_is_clicked) {
        this->clicked_node = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
        if (clicked_node) {
            this->node_is_clicked = true;
        }
        else {
            return;
        }
        while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
            while (!dst || dst == clicked_node) {
                std::shared_ptr<Node> tmp_node = std::make_shared<Node>(Node("tmp", MOUSE_HOVER_X_CORRECTED, MOUSE_HOVER_Y_CORRECTED));
                std::shared_ptr<Edge> tmp_edge = std::make_shared<Edge>(Edge(this->clicked_node, tmp_node, false));
                this->window->clear(BG_COLOR);
                this->toolbar.render(*this->window);
                this->graph.render(*this->window);
                tmp_edge->render(*this->window);
                this->window->display();
                dst = this->graph.getNodeByPosition(MOUSE_HOVER_X, MOUSE_HOVER_Y);
                this->window->pollEvent(this->sfEvent);
                if (this->sfEvent.type == sf::Event::MouseButtonReleased) {
                    if (dst == clicked_node) {
                        if (edge_was_added) {
                            this->clicked_node = nullptr;
                            this->node_is_clicked = false;
                        }
                        return;
                    }
                    else if (!dst) {
                        edge_was_added = false;
                    }
                    else {
                        edge_was_added = true;
                    }
                    break;
                }
            }
            if (!edge_was_added && !dst) {
                this->clicked_node = nullptr;
                this->node_is_clicked = false;
                return;
            }
            if (dst && dst != clicked_node) {
                std::shared_ptr<Edge> to_add = std::make_shared<Edge>(this->clicked_node, dst);
                this->graph.addEdge(to_add);
                this->clicked_node = dst;
                this->graph.setToggledNode(dst);
                this->window->clear(BG_COLOR);
                this->toolbar.render(*this->window);
                this->graph.render(*this->window);
                to_add->render(*this->window);
                this->window->display();
                edge_was_added = true;
            }
            this->window->pollEvent(this->sfEvent);
        }
    }
    else {
        dst = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
        if (dst && dst != clicked_node) {
            std::shared_ptr<Edge> to_add = std::make_shared<Edge>(this->clicked_node, dst);
            this->graph.addEdge(to_add);
            edge_was_added = true;
        }
        this->node_is_clicked = false;
        this->clicked_node = nullptr;
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && edge_was_added) {
        this->graph.reset();
    }
}




void Visualizer::eraseRoutine() {
    std::shared_ptr<Node> node_to_delete = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
    std::shared_ptr<Edge> edge_to_delete = this->graph.getEdgeByPosition(MOUSE_X, MOUSE_Y);
    if (node_to_delete) {
        this->graph.removeNode(node_to_delete->getName());
    }
    else if (edge_to_delete) {
        this->graph.removeEdge(edge_to_delete);
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && (node_to_delete || edge_to_delete)) {
        this->graph.reset();
    }
}


void Visualizer::changeStartNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists) {
        this->graph.reset();
    }
    this->graph.setStartNode(this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y));
}


void Visualizer::runBFSRoutine() {
    if (algo_thread_is_running || !is_immediate) return;
    sf::Vector2i original_active_button_coordinates = sf::Vector2i(MOUSE_X, MOUSE_Y);
    while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
        this->window->pollEvent(this->sfEvent);
    }
    if (!this->toolbar.updateActiveButton(sf::Vector2i(MOUSE_X, MOUSE_Y)) || this->toolbar.getActiveButtonId() != RUN_BFS) {
        this->toolbar.updateActiveButton(original_active_button_coordinates);
        return;
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
        this->graph.reset();
    }
    this->mode = BFS;
    runAlgorithm();
}


void Visualizer::runDfSRoutine() {
    if (algo_thread_is_running || !is_immediate) return;
    sf::Vector2i original_active_button_coordinates = sf::Vector2i(MOUSE_X, MOUSE_Y);
    while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
        this->window->pollEvent(this->sfEvent);
    }
    if (!this->toolbar.updateActiveButton(sf::Vector2i(MOUSE_X, MOUSE_Y)) || this->toolbar.getActiveButtonId() != RUN_DFS) {
        this->toolbar.updateActiveButton(original_active_button_coordinates);
        return;
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
        this->graph.reset();
    }
    this->mode = DFS;
    runAlgorithm();
}


void Visualizer::endRoutine() {
    if (!algo_thread_is_running) return;
    should_end = true;
    algo_thread.join();
    is_finished = false;
    should_end = false;

    this->graph.reset();
    if (this->mode == BFS) {
        this->graph.runBFS(*this->window, this->toolbar, false);
    }
    else if (this->mode == DFS) {
        this->graph.runDFS(*this->window, this->toolbar, false);
    }
    else {
        this->graph.runDijkstra(*this->window, this->toolbar, false);
    }
}


void Visualizer::resetRoutine() {
    if (algo_thread_is_running) {
        should_end = true;
        algo_thread.join();
        is_finished = false;
        should_end = false;
    }
    this->graph.reset();
}


void Visualizer::clearWindowRoutine() {
    if (algo_thread_is_running) {
        should_end = true;
        algo_thread.join();
        is_finished = false;
        should_end = false;
    }
    this->graph = Graph();
    this->node_is_clicked = false;
    this->clicked_node = nullptr;
    this->toolbar.resetActiveButton();
}

