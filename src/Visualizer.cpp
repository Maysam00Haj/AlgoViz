#include "Visualizer.h"
#include "utils.h"
#include "Node.h"
#include "TextBox.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <windows.h>


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


#define ROWS    (83/this->current_zoom_factor)
#define COLS    ((83*((float)this->window->getView().getSize().x/(float)this->window->getView().getSize().y)) \
/this->current_zoom_factor)

#define MAX_ZOOM    5
#define MIN_ZOOM    0.3



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
    this->vis_font->loadFromFile("fonts/Raleway-Regular.ttf");

    this->window = new sf::RenderWindow(sf::VideoMode(1400, 1000), "Graph Visualizer");
    this->window->setFramerateLimit(120);

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
                this->current_zoom_factor = 1;
                break;
            }
            case sf::Event::MouseWheelScrolled: {
                if (this->sfEvent.mouseWheelScroll.delta > 0 && current_zoom_factor < MAX_ZOOM) {
                    this->current_view.zoom(0.8);
                    this->current_zoom_factor *= 1.25;
                }
                else if (this->sfEvent.mouseWheelScroll.delta < 0 && current_zoom_factor > MIN_ZOOM){
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
    this->drawGrid();
    this->graph.render(*this->window, this->vis_font);
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
    this->window->setView(this->current_view);
    if (this->toolbar.getActiveButtonId() == ADD_EDGE && this->node_is_clicked) {
        std::shared_ptr<Node> tmp_node = std::make_shared<Node>(Node("tmp", CORRECTED_MOUSE_X, CORRECTED_MOUSE_Y, vis_font));
        std::shared_ptr<Edge> tmp_edge = std::make_shared<Edge>(Edge(this->clicked_node, tmp_node, false));
        tmp_edge->render(*this->window);
    }
    this->window->display();
    window->setActive(false);
    window_lock.unlock();
}



void Visualizer::drawGrid() {
    // initialize values
    int numLines = ROWS+COLS-2;
    sf::VertexArray grid(sf::Lines, 2*(numLines));
    auto size = this->original_view.getSize();
    size.x *= 5;
    size.y *= 5;
    float rowH = 2 * NODE_RADIUS * (this->current_zoom_factor);
    float colW = 2 * NODE_RADIUS * (this->current_zoom_factor);
    // row separators
    for(int i=0; i < ROWS-1; i++){
        int r = i+1;
        float rowY = rowH*r;
        grid[i*2].position = {-size.x/2, rowY-size.y/2};
        grid[i*2].color = sf::Color(255,255,255,20);
        grid[i*2+1].position = {size.x/2, rowY-size.y/2};
        grid[i*2+1].color = sf::Color(255,255,255,20);
    }
    // column separators
    for(int i=ROWS-1; i < numLines; i++){
        int c = i-ROWS+2;
        float colX = colW*c;
        grid[i*2].position = {colX-size.x/2, -size.y/2};
        grid[i*2].color = sf::Color(255,255,255,20);
        grid[i*2+1].position = {colX-size.x/2, size.y/2};
        grid[i*2+1].color = sf::Color(255,255,255,20);
    }
    // draw it
    this->window->draw(grid);
}



void Visualizer::executeClickAction() {
    //Didn't use macros because this case is unique, other class functions work on position relative to window
    // and not global or world position:
    auto window_x = (float)this->sfEvent.mouseButton.x;
    auto window_y = (float)this->sfEvent.mouseButton.y;

    if (this->messagesBox.inBoundsMinimize(window_x, window_y)) {
        this->messagesBox.minimizeBox();
        return;
    }

    if (this->messagesBox.inBoundsMaximize(window_x, window_y)) {
        this->messagesBox.maximizeBox();
        return;
    }

    is_immediate = this->toolbar.updateActiveButton(sf::Vector2f(window_x,window_y));
    ButtonId id = this->toolbar.getActiveButtonId();

    switch (id) {
        case REMOVE_TARGET_NODE: {
            removeTargetNodeRoutine();
            break;
        }
        case RUN_BFS: {
            this->mode = BFS;
            runAlgorithm();
            break;
        }
        case RUN_DFS: {
            this->mode = DFS;
            runAlgorithm();
            break;
        }
        case RUN_DIJKSTRA: {
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
        case SAVE_TO_FILE: {
            saveToFile();
            break;
        }
        case LOAD_FROM_FILE: {
            loadFromFile();
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
    auto window_x = (float) this->sfEvent.mouseButton.x;
    auto window_y = (float) this->sfEvent.mouseButton.y;
    sf::Vector2f original_active_button_coordinates = sf::Vector2f(window_x, window_y);
    while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
        this->window->pollEvent(this->sfEvent);
    }
    if (!this->toolbar.updateActiveButton(sf::Vector2f(window_x, window_y))) {
        this->toolbar.updateActiveButton(original_active_button_coordinates);
        return;
    }
    if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() == NODE_DONE || this->graph.getStartNode()->getState() == NODE_NEAREST)) {
        this->graph.reset();
    }
    this->toolbar.resetActiveButton();

    VisMode current_mode = this->mode;
    bool should_wait = true;
    switch (current_mode) {
        case BFS: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runBFS, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), std::ref(this->original_view),
                                      std::ref(this->current_view), std::ref(this->vis_font), should_wait);
            break;
        }
        case DFS: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runDFS, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), std::ref(this->original_view),
                                      std::ref(this->current_view), std::ref(this->vis_font), should_wait);
            break;
        }
        case DIJKSTRA: {
            this->window->setActive(false);
            algo_thread = std::thread(&Graph::runDijkstra, std::ref(this->graph), std::ref(*this->window),
                                      std::ref(this->toolbar), std::ref(this->original_view),
                                      std::ref(this->current_view), std::ref(this->vis_font), should_wait);
            break;
        }
    }
}




void Visualizer::cursorRoutine() {
    std::shared_ptr<Node> moving_node = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (!moving_node) {
        // didn't click on a node, which means we're moving the view
        float window_x = (float)sf::Mouse::getPosition().x;
        float window_y = (float)sf::Mouse::getPosition().y;
        sf::Vector2f prev_pos = sf::Vector2f (window_x, window_y);
        sf::Vector2f current_pos, delta_pos;
        while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
            window_x = (float) sf::Mouse::getPosition().x;
            window_y = (float) sf::Mouse::getPosition().y;
            current_pos = sf::Vector2f(window_x, window_y);
            delta_pos = prev_pos - current_pos; // inverted because when we "look" to the left things "move" to the right
            this->current_view.move(delta_pos / this->current_zoom_factor);
            prev_pos = current_pos;
            this->render();
            this->window->pollEvent(this->sfEvent);
        }
        return;
    }

    // Clicked on a node, which means we're moving a node:
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
        this->render();
        this->window->pollEvent(this->sfEvent);
    }
}


void Visualizer::addNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.addNode(CORRECTED_EVENT_X, CORRECTED_EVENT_Y, vis_font);
    if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() == NODE_DONE || this->graph.getStartNode()->getState() == NODE_NEAREST) && node_exists) {
        this->graph.reset();
    }
    if (graph.getStartNode() != nullptr) {
        this->messagesBox.is_rendered[EMPTY_GRAPH_M] = false;
    }
}


void Visualizer::addEdgeRoutine() {
    bool edge_was_added = false;
    std::shared_ptr<Node> dst = nullptr;

    if (this->node_is_clicked) {
        // a node was clicked and the mouse was released
        // the next node to be clicked will be connected by an edge to the already clicked one
        dst = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
        if (dst && dst != clicked_node) {
            std::shared_ptr<Edge> to_add = std::make_shared<Edge>(this->clicked_node, dst);
            this->graph.addEdge(to_add);
            edge_was_added = true;
        }
        this->node_is_clicked = false;
        this->clicked_node = nullptr;
        if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() == NODE_DONE || this->graph.getStartNode()->getState() == NODE_NEAREST) && edge_was_added) {
            this->graph.reset();
        }
        return;
    }
    // The following is in case no node was previously clicked to draw an edge
    // so drawing an edge is going to happen by selecting a node after clicking
    // and connecting it to another node that is going to be selected after releasing
    this->clicked_node = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (!clicked_node) return;
    this->node_is_clicked = true;

    while (this->sfEvent.type != sf::Event::MouseButtonReleased) {
        // As long as the mouse wasn't released:
        while (!dst || dst == clicked_node) {
            // Keep rendering an edge animation that follows the mouse from the clicked node:
            this->render();
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
            this->render();
            edge_was_added = true;
        }
        this->window->pollEvent(this->sfEvent);
    }
    if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() == NODE_DONE || this->graph.getStartNode()->getState() == NODE_NEAREST) && edge_was_added) {
        this->graph.reset();
    }
}




void Visualizer::eraseRoutine() {
    std::shared_ptr<Node> node_to_delete = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    std::shared_ptr<Edge> edge_to_delete = this->graph.getEdgeByPosition(EVENT_X, EVENT_Y);
    if (node_to_delete) {
        this->graph.removeNode(node_to_delete->getName());
        if (graph.getStartNode() == nullptr) {
            this->messagesBox.is_rendered[EMPTY_GRAPH_M] = true;
        }
    }
    else if (edge_to_delete) {
        this->graph.removeEdge(edge_to_delete);
    }
    if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() == NODE_DONE || this->graph.getStartNode()->getState() == NODE_NEAREST) && (node_to_delete || edge_to_delete)) {
        this->graph.reset();
    }
}


void Visualizer::changeStartNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() || this->graph.getStartNode()->getState() == NODE_NEAREST) && node_exists) {
        this->graph.reset();
    }
    this->graph.setStartNode(this->graph.getNodeByPosition(EVENT_X, EVENT_Y));
}

void Visualizer::chooseTargetNodeRoutine() {
    std::shared_ptr<Node> node_exists = this->graph.getNodeByPosition(EVENT_X, EVENT_Y);
    if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() == NODE_DONE || this->graph.getStartNode()->getState() == NODE_NEAREST) && node_exists) {
        this->graph.reset();
    }
    this->graph.setTargetNode(this->graph.getNodeByPosition(EVENT_X, EVENT_Y));
}

void Visualizer::removeTargetNodeRoutine() {
    if (algo_thread_is_running || !is_immediate) return;
    if (this->graph.getStartNode() && (this->graph.getStartNode()->getState() == NODE_DONE || this->graph.getStartNode()->getState() == NODE_NEAREST)) {
        this->graph.reset();
    }
    this->graph.removeTargetNode();
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
        this->graph.runBFS(*this->window, this->toolbar, this->original_view, this->current_view, this->vis_font, false);
    }
    else if (this->mode == DFS) {
        this->graph.runDFS(*this->window, this->toolbar, this->original_view, this->current_view, this->vis_font, false);
    }
    else {
        this->graph.runDijkstra(*this->window, this->toolbar, this->original_view, this->current_view, this->vis_font, false);
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



void Visualizer::saveToFile() {
    std::string graph_name;
    this->window->pollEvent(this->sfEvent);
    InputBox inputBox(*(this->window), this->vis_font);
    this->window->setView(this->original_view);
    graph_name = inputBox.getInput(*(this->window));

    if (graph_name.empty() || std::find(this->saved_graphs.begin(), this->saved_graphs.end(), graph_name) != this->saved_graphs.end()) {
        this->toolbar.resetActiveButton();
        return;
    }

    std::ofstream save_file;
    save_file.open("SavedGraphs.txt", std::ios::out | std::ios::app);
    std::string graph_encoding = this->graph.getEncoding();
    graph_encoding = "svg:" + graph_name + graph_encoding;
    save_file.write(graph_encoding.c_str(), (long long)graph_encoding.size());
    this->toolbar.resetActiveButton();
}



void Visualizer::loadFromFile() {

}

void Visualizer::deleteFromFile() {

}

