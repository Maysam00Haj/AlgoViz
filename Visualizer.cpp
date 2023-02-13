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

std::thread algo_thread; // the thread we use to execute runBfs




Visualizer::Visualizer(const Graph& graph): graph(graph) {
    this->window = new sf::RenderWindow(sf::VideoMode(1000, 600), "Graph Visualizer");
    this->window->setFramerateLimit(60);
}

Visualizer::~Visualizer() {
  delete this->window;
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

void Visualizer::run() {
    while (this->window->isOpen()) {
        this->update();
        if (!algo_thread_is_running) {
            this->render();
        }
    }
}


void Visualizer::executeClickAction() {
    bool is_immediate = this->toolbar.updateActiveButton(sf::Vector2i(MOUSE_X, MOUSE_Y));
    ButtonId id = this->toolbar.getActiveButtonId();

    switch (id) {
        case RUN_BFS: {
            if (algo_thread_is_running || !is_immediate) break;
            sf::Vector2i original_active_button_coordinates = sf::Vector2i(MOUSE_X, MOUSE_Y);
            while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
                this->window->pollEvent(this->sfEvent);
            }
            if (!this->toolbar.updateActiveButton(sf::Vector2i(MOUSE_X, MOUSE_Y)) || this->toolbar.getActiveButtonId() != RUN_BFS) {
                this->toolbar.updateActiveButton(original_active_button_coordinates);
                break;
            }
            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
                this->graph.reset();
            }
            this->mode = BFS;
            runAlgorithm();
            break;
        }
        case RUN_DFS: {
            if (algo_thread_is_running || !is_immediate) break;
            sf::Vector2i original_active_button_coordinates = sf::Vector2i(MOUSE_X, MOUSE_Y);
            while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
                this->window->pollEvent(this->sfEvent);
            }
            if (!this->toolbar.updateActiveButton(sf::Vector2i(MOUSE_X, MOUSE_Y)) || this->toolbar.getActiveButtonId() != RUN_DFS) {
                this->toolbar.updateActiveButton(original_active_button_coordinates);
                break;
            }
            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
                this->graph.reset();
            }
            this->mode = DFS;
            runAlgorithm();
            break;
        }
        case END: {
            if (!algo_thread_is_running) break;
            should_end = true;
            algo_thread.join();
            is_finished = false;
            should_end = false;

            this->graph.reset();

            if (this->mode == BFS)
                this->graph.runBFS(*this->window, this->toolbar, false);
            else
                this->graph.runDFS(*this->window, this->toolbar, false);

            break;
        }
        case RESET: {
            if (algo_thread_is_running) {
                should_end = true;
                algo_thread.join();
                is_finished = false;
                should_end = false;
            }
            this->graph.reset();
            break;
        }
        case CLEAR_WINDOW: {
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
        case ADD_NODE: {
            std::shared_ptr<Node> node_exists = this->graph.addNode(MOUSE_X_CORRECTED, MOUSE_Y_CORRECTED);
            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists)
                this->graph.reset();
            break;
        }
        case ADD_EDGE: {
            bool action_was_performed = false;
            if (!this->node_is_clicked) {
                this->clicked_node = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
                if (clicked_node) {
                    this->node_is_clicked = true;
                }
                while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
                    std::shared_ptr<Node> dst = this->graph.getNodeByPosition(MOUSE_HOVER_X, MOUSE_HOVER_Y);
                    if (dst && dst != clicked_node) {
                        std::shared_ptr<Edge> to_add = std::make_shared<Edge>(this->clicked_node, dst);
                        this->graph.addEdge(to_add);
                        action_was_performed = true;
                        this->clicked_node = dst;
                        this->graph.setToggledNode(dst);
                    }
                    this->window->pollEvent(this->sfEvent);
                }
                if (action_was_performed) {
                    this->node_is_clicked = false;
                    this->clicked_node = nullptr;
                }
            }
            else {
                std::shared_ptr<Node> dst = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
                if (dst && dst != clicked_node) {
                    std::shared_ptr<Edge> to_add = std::make_shared<Edge>(this->clicked_node, dst);
                    this->graph.addEdge(to_add);
                    action_was_performed = true;
                }
                this->node_is_clicked = false;
                this->clicked_node = nullptr;
            }

            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && action_was_performed)
                this->graph.reset();
            break;
        }
        case ERASE: {
            std::shared_ptr<Node> node_to_delete = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
            std::shared_ptr<Edge> edge_to_delete = this->graph.getEdgeByPosition(MOUSE_X, MOUSE_Y);
            if (node_to_delete) {
                this->graph.removeNode(node_to_delete->getName());
            }
            else if (edge_to_delete) {
                this->graph.removeEdge(edge_to_delete);
            }
            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && (node_to_delete || edge_to_delete))
                this->graph.reset();
            break;
        }
        case CHANGE_START_NODE: {
            std::shared_ptr<Node> node_exists = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
            if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists)
                this->graph.reset();

            this->graph.setStartNode(this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y));
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


