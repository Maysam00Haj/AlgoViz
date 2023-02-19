#include "Visualizer.h"
#include "utils.h"
#include <thread>
#include <mutex>
#include "Node.h"

//#define MOUSE_X       (this->sfEvent.mouseButton.x)
//#define MOUSE_Y       (this->sfEvent.mouseButton.y)
//#define MOUSE_HOVER_X ((float)sf::Mouse::getPosition(*this->window).x)
//#define MOUSE_HOVER_Y ((float)sf::Mouse::getPosition(*this->window).y)
//
//
//#define MOUSE_X_CORRECTED       (this->sfEvent.mouseButton.x-30)
//#define MOUSE_Y_CORRECTED       (this->sfEvent.mouseButton.y-30)
//#define MOUSE_HOVER_X_CORRECTED ((float)sf::Mouse::getPosition(*this->window).x-30)
//#define MOUSE_HOVER_Y_CORRECTED ((float)sf::Mouse::getPosition(*this->window).y-30)

#define EVENT_X (this->window->mapPixelToCoords( \
                sf::Vector2i(this->sfEvent.mouseButton.x,this->sfEvent.mouseButton.y)).x)

#define EVENT_Y (this->window->mapPixelToCoords( \
                sf::Vector2i(this->sfEvent.mouseButton.x,this->sfEvent.mouseButton.y)).y)

#define MOUSE_X (this->window->mapPixelToCoords( \
                sf::Vector2i((float)sf::Mouse::getPosition(*this->window).x, \
                (float)sf::Mouse::getPosition(*this->window).y)).x)

#define MOUSE_Y (this->window->mapPixelToCoords( \
                sf::Vector2i((float)sf::Mouse::getPosition(*this->window).x, \
                (float)sf::Mouse::getPosition(*this->window).y)).y)

#define CORRECTED_EVENT_X   (this->window->mapPixelToCoords( \
                            sf::Vector2i(this->sfEvent.mouseButton.x-(30*this->current_zoom_factor), \
                            this->sfEvent.mouseButton.y-(30*this->current_zoom_factor))).x)

#define CORRECTED_EVENT_Y   (this->window->mapPixelToCoords( \
                            sf::Vector2i(this->sfEvent.mouseButton.x-(30*this->current_zoom_factor), \
                            this->sfEvent.mouseButton.y-(30*this->current_zoom_factor))).y)

#define CORRECTED_MOUSE_X   (this->window->mapPixelToCoords( \
                            sf::Vector2i((float)sf::Mouse::getPosition(*this->window).x-(30*this->current_zoom_factor), \
                            (float)sf::Mouse::getPosition(*this->window).y-(30*this->current_zoom_factor))).x)

#define CORRECTED_MOUSE_Y   (this->window->mapPixelToCoords( \
                            sf::Vector2i((float)sf::Mouse::getPosition(*this->window).x-(30*this->current_zoom_factor), \
                            (float)sf::Mouse::getPosition(*this->window).y-(30*this->current_zoom_factor))).y)



#define CHECK_THREAD_AND_JOIN \
if (is_finished) {            \
    algo_thread.join();       \
    is_finished = false;      \
}


std::mutex window_lock; // prevents multiple threads from accessing window resource
bool algo_thread_is_running = false; // true when a thread is currently running runBfs
bool is_finished = false; // true when a thread ran and ended execution of runBfs, so we need to call join()
bool should_end = false;

bool is_immediate = false;

std::thread algo_thread; // the thread we use to execute runBfs



Visualizer::Visualizer(const Graph& graph): graph(graph) {
    this->vis_font = new sf::Font();
    this->vis_font->loadFromFile("fonts/arial.ttf");

    this->window = new sf::RenderWindow(sf::VideoMode(1400, 1000), "Graph Visualizer");
    this->window->setFramerateLimit(60);

    this->messagesBox = MessagesBox(vis_font);

    this->current_view = this->window->getView();
    this->original_view = this->window->getDefaultView();
}



Visualizer::~Visualizer() {
  delete this->window;
  delete this->vis_font;
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
                sf::FloatRect view(0, 0, (float)this->sfEvent.size.width, (float)this->sfEvent.size.height);
                this->current_view = sf::View(view);
                this->original_view = sf::View(view);
                this->window->setView(this->current_view);
                break;
            }
            case sf::Event::MouseWheelScrolled: {
                if (this->sfEvent.mouseWheelScroll.delta > 0) {
                    this->current_view.zoom(0.8);
                    this->current_zoom_factor *= 1.25;
                }
                else if (this->sfEvent.mouseWheelScroll.delta < 0){
                    this->current_view.zoom(1.25);
                    this->current_zoom_factor *= 0.8;
                }
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
    window->setView(this->current_view);
    this->graph.render(*this->window);
    this->window->setView(this->original_view);
    this->toolbar.render(*this->window);
    this->messagesBox.render(*this->window);
    if (this->toolbar.getActiveButtonId() == ADD_NODE) {
        float corrected_radius = this->current_zoom_factor * NODE_RADIUS;
        sf::CircleShape hover_node(corrected_radius);
        hover_node.setFillColor(sf::Color(255, 255, 255, 100));
        hover_node.setPosition(CORRECTED_MOUSE_X, CORRECTED_MOUSE_Y);
        this->window->draw(hover_node);
    }
    if (this->toolbar.getActiveButtonId() == ADD_EDGE && this->node_is_clicked) {
        std::shared_ptr<Node> tmp_node = std::make_shared<Node>(Node("tmp", CORRECTED_MOUSE_X, CORRECTED_MOUSE_Y, vis_font));
        std::shared_ptr<Edge> tmp_edge = std::make_shared<Edge>(Edge(this->clicked_node, tmp_node, false));
        tmp_edge->render(*this->window);
    }
    this->window->setView(this->current_view);
    this->window->display();
    window->setActive(false);
    window_lock.unlock();
}




void Visualizer::executeClickAction() {
    //Didn't use macros because this case is unique, other class functions work on position relative to window
    // and not global or world position:
    auto window_x = (float)this->sfEvent.mouseButton.x;
    auto window_y = (float)this->sfEvent.mouseButton.y;
    is_immediate = this->toolbar.updateActiveButton(sf::Vector2f(window_x,window_y));
    ButtonId id = this->toolbar.getActiveButtonId();

    switch (id) {
        case REMOVE_TARGET_NODE: {
            removeTargetNodeRoutine();
            break;
        }
        case RUN_BFS: {
            runBFSRoutine();
            break;
        }
        case RUN_DFS: {
            runDfSRoutine();
            break;
        }
        case RUN_DIJKSTRA: {
            runDijkstraRoutine();
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
    std::shared_ptr<Node> to_toggle = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
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
        case CHOOSE_TARGET_NODE: {
            chooseTargetNodeRoutine();
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
                                      std::ref(this->toolbar), std::ref(this->original_view), std::ref(this->current_view), should_wait);
            break;
        }
        case DFS: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runDFS, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), std::ref(this->original_view), std::ref(this->current_view), should_wait);
            break;
        }
        case DIJKSTRA: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runDijkstra, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), std::ref(this->original_view), std::ref(this->current_view), should_wait);
            break;
        }
    }
}




void Visualizer::cursorRoutine() {
    std::shared_ptr<Node> moving_node = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (moving_node) {
        while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
            moving_node->setPosition(CORRECTED_MOUSE_X, CORRECTED_MOUSE_Y);
            if (!this->graph.checkValidPosition(*moving_node)) {
                std::shared_ptr<Node> collided_node = this->graph.getCollidedNode(moving_node);
                if (collided_node) {
                    float collided_x = collided_node->getPosition().x;
                    float collided_y = collided_node->getPosition().y;
                    std::vector<float> closest_pos = getClosestNonCollision(collided_x, collided_y,
                                                                            CORRECTED_MOUSE_X,
                                                                            CORRECTED_MOUSE_Y);
                    moving_node->setPosition(closest_pos[0], closest_pos[1]);
                    if (!this->graph.checkValidPosition(*moving_node)) continue;
                }
            }
            this->window->clear(BG_COLOR);
            window->setView(this->current_view);
            this->graph.render(*this->window);
            this->window->setView(this->original_view);
            this->toolbar.render(*this->window);
            this->messagesBox.render(*this->window);
            this->window->setView(this->current_view);
            this->window->display();
            this->window->pollEvent(this->sfEvent);
        }
    }
    else {
        float window_x = (float)sf::Mouse::getPosition().x;
        float window_y = (float)sf::Mouse::getPosition().y;
        sf::Vector2f prev_pos = sf::Vector2f (window_x, window_y);
        sf::Vector2f current_pos, delta_pos;
        while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
            window_x = (float)sf::Mouse::getPosition().x;
            window_y = (float)sf::Mouse::getPosition().y;
            current_pos = sf::Vector2f (window_x, window_y);
            delta_pos = prev_pos - current_pos; // inverted because when we "look" to the left things "move" to the right
            this->current_view.move(delta_pos / this->current_zoom_factor);
            prev_pos = current_pos;
            this->window->clear(BG_COLOR);
            window->setView(this->current_view);
            this->graph.render(*this->window);
            this->window->setView(this->original_view);
            this->toolbar.render(*this->window);
            this->messagesBox.render(*this->window);
            this->window->display();
            this->window->pollEvent(this->sfEvent);
        }
    }
}


void Visualizer::addNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.addNode(CORRECTED_EVENT_X, CORRECTED_EVENT_Y, vis_font);
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists) {
        this->graph.reset();
    }
    if (graph.getStartNode() != nullptr) {
        this->messagesBox.is_rendered[0] = false;
    }
}


void Visualizer::addEdgeRoutine() {
    bool edge_was_added = false;
    std::shared_ptr<Node> dst = nullptr;
    if (!this->node_is_clicked) {
        this->clicked_node = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
        if (clicked_node) {
            this->node_is_clicked = true;
        }
        else {
            return;
        }
        while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
            while (!dst || dst == clicked_node) {
                std::shared_ptr<Node> tmp_node = std::make_shared<Node>(Node("tmp", CORRECTED_MOUSE_X, CORRECTED_MOUSE_Y, vis_font));
                std::shared_ptr<Edge> tmp_edge = std::make_shared<Edge>(Edge(this->clicked_node, tmp_node, false));
                this->window->clear(BG_COLOR);
                window->setView(this->current_view);
                this->graph.render(*this->window);
                this->window->setView(this->original_view);
                this->toolbar.render(*this->window);
                this->messagesBox.render(*this->window);
                this->window->setView(this->current_view);
                tmp_edge->render(*this->window);
                this->window->display();
                dst = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
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
                window->setView(this->current_view);
                this->graph.render(*this->window);
                this->window->setView(this->original_view);
                this->toolbar.render(*this->window);
                this->messagesBox.render(*this->window);
                this->window->setView(this->current_view);
                to_add->render(*this->window);
                this->window->display();
                edge_was_added = true;
            }
            this->window->pollEvent(this->sfEvent);
        }
    }
    else {
        dst = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
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
    std::shared_ptr<Node> node_to_delete = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    std::shared_ptr<Edge> edge_to_delete = this->graph.getEdgeByPosition(EVENT_X, EVENT_Y);
    if (node_to_delete) {
        this->graph.removeNode(node_to_delete->getName());
        if (graph.getStartNode() == nullptr) {
            this->messagesBox.is_rendered[0] = true;
        }
    }
    else if (edge_to_delete) {
        this->graph.removeEdge(edge_to_delete);
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && (node_to_delete || edge_to_delete)) {
        this->graph.reset();
    }
}


void Visualizer::changeStartNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists) {
        this->graph.reset();
    }
    this->graph.setStartNode(this->graph.getNodeByPosition(EVENT_X, EVENT_Y));
}

void Visualizer::chooseTargetNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists) {
        this->graph.reset();
    }
    this->graph.setTargetNode(this->graph.getNodeByPosition(EVENT_X, EVENT_Y));
}

void Visualizer::removeTargetNodeRoutine() {
    if (algo_thread_is_running || !is_immediate) return;
    std::shared_ptr<Node> node_exists = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE && node_exists) {
        this->graph.reset();
    }
    this->graph.removeTargetNode();
    this->toolbar.resetActiveButton();
}


void Visualizer::runBFSRoutine() {
    if (algo_thread_is_running || !is_immediate) return;
    auto window_x = (float) this->sfEvent.mouseButton.x;
    auto window_y = (float) this->sfEvent.mouseButton.y;
    sf::Vector2f original_active_button_coordinates = sf::Vector2f(window_x, window_y);
    while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
        this->window->pollEvent(this->sfEvent);
    }
    if (!this->toolbar.updateActiveButton(sf::Vector2f(window_x, window_y)) || this->toolbar.getActiveButtonId() != RUN_BFS) {
        this->toolbar.updateActiveButton(original_active_button_coordinates);
        return;
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
        this->graph.reset();
    }
    this->mode = BFS;
    runAlgorithm();
    this->toolbar.resetActiveButton();
}



void Visualizer::runDfSRoutine() {
    if (algo_thread_is_running || !is_immediate) return;
    auto window_x = (float) this->sfEvent.mouseButton.x;
    auto window_y = (float) this->sfEvent.mouseButton.y;
    sf::Vector2f original_active_button_coordinates = sf::Vector2f(window_x, window_y);
    while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
        this->window->pollEvent(this->sfEvent);
    }
    if (!this->toolbar.updateActiveButton(sf::Vector2f(window_x, window_y)) || this->toolbar.getActiveButtonId() != RUN_DFS) {
        this->toolbar.updateActiveButton(original_active_button_coordinates);
        return;
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
        this->graph.reset();
    }
    this->mode = DFS;
    runAlgorithm();
    this->toolbar.resetActiveButton();
}



void Visualizer::runDijkstraRoutine() {
    if (algo_thread_is_running || !is_immediate) return;
    auto window_x = (float) this->sfEvent.mouseButton.x;
    auto window_y = (float) this->sfEvent.mouseButton.y;
    sf::Vector2f original_active_button_coordinates = sf::Vector2f(window_x, window_y);
    while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
        this->window->pollEvent(this->sfEvent);
    }
    if (!this->toolbar.updateActiveButton(sf::Vector2f(window_x, window_y)) || this->toolbar.getActiveButtonId() != RUN_DIJKSTRA) {
        this->toolbar.updateActiveButton(original_active_button_coordinates);
        return;
    }
    if (this->graph.getStartNode() && this->graph.getStartNode()->getState() == NODE_DONE) {
        this->graph.reset();
    }
    this->mode = DIJKSTRA;
    runAlgorithm();
    this->toolbar.resetActiveButton();
}



void Visualizer::endRoutine() {
    this->toolbar.resetActiveButton();
    if (!algo_thread_is_running) return;
    should_end = true;
    algo_thread.join();
    is_finished = false;
    should_end = false;

    this->graph.reset();
    if (this->mode == BFS) {
        this->graph.runBFS(*this->window, this->toolbar, this->original_view, this->current_view, false);
    }
    else if (this->mode == DFS) {
        this->graph.runDFS(*this->window, this->toolbar, this->original_view, this->current_view, false);
    }
    else {
        this->graph.runDijkstra(*this->window, this->toolbar, this->original_view, this->current_view, false);
    }
    this->toolbar.resetActiveButton();
}


void Visualizer::resetRoutine() {
    if (algo_thread_is_running) {
        should_end = true;
        algo_thread.join();
        is_finished = false;
        should_end = false;
    }
    this->graph.reset();
    this->toolbar.resetActiveButton();
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

void Visualizer::loadFromFile() {

}

void Visualizer::storeInFile() const {

}
