#include "Visualizer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

#define MOUSE_X (this->sfEvent.mouseButton.x)
#define MOUSE_Y (this->sfEvent.mouseButton.y)

// Use these only for drawing shapes (shapes drawn to wrong part of cursor)
#define MOUSE_X_CORRECTED (this->sfEvent.mouseButton.x-30)
#define MOUSE_Y_CORRECTED (this->sfEvent.mouseButton.y-30)

std::mutex run_lock; // prevents multiple threads from executing runBfs method at the same time
std::mutex window_lock; // prevents multiple threads from accessing window resource
bool algo_thread_is_running = false; // true when a thread is currently running runBfs
bool is_finished = false; // true when a thread ran and ended execution of runBfs, so we need to call join()

std::thread algo_thread; // the thread we use to execute runBfs

Visualizer::Visualizer() {
    this->window = new sf::RenderWindow(sf::VideoMode(1000, 600), "Graph Visualizer");
    this->window->setFramerateLimit(60);
}

Visualizer::~Visualizer() {
  delete this->window;
}

void Visualizer::update() {
    window_lock.lock();
    int event_type = this->window->pollEvent(this->sfEvent);
    window_lock.unlock();
    while(event_type) {
        if (is_finished) {
            algo_thread.join();
        }
        switch (this->sfEvent.type) {
            case sf::Event::Closed: {
                if (is_finished) {
                    algo_thread.join();
                }
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
        window_lock.lock();
        event_type = this->window->pollEvent(this->sfEvent);
        window_lock.unlock();
    }
}

void Visualizer::render() {
    window_lock.lock();
    window->setActive(true);
    this->window->clear();
    this->toolbar.render(*this->window);
    this->graph.render(*this->window);
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
    bool is_immediate = false;
    for (const auto &button: this->toolbar.buttons) {
        if (button->update(sf::Vector2i(MOUSE_X, MOUSE_Y))) {
            this->toolbar.update(sf::Vector2i(MOUSE_X, MOUSE_Y));
            is_immediate = true;
            break;
        }
    }
    button_id id = this->toolbar.getActiveButtonId();

    switch (id) {
        case START: {
            if (algo_thread_is_running) break;
            runAlgorithm();
            break;
        }
        case END: {
            //TODO: change implementation so that it ends runBFS immediately suggestion: use global variable to end algo_thread, and run runBFS without stop
            if (algo_thread_is_running) {
                algo_thread.join();
                is_finished = false;
            }
            break;
        }
        case RESET: {
            //TODO: same concept and end and clear_window
            this->graph.reset();
            break;
        }
        case CLEAR_WINDOW: {
            //TODO: end thread run and clear window immediately, same concept and end.
            this->graph = Graph();
            this->node_is_clicked = false;
            this->clicked_node = nullptr;
            this->toolbar.updateActiveButton();
            break;
        }
        default: {
            break;
        }

    }

    if (is_immediate || algo_thread_is_running) return;
    // the following cases are not allowed while an algorithm is running

    switch (id) {
        case ADD_NODE: {
            this->graph.addNode(MOUSE_X_CORRECTED, MOUSE_Y_CORRECTED);
            break;
        }
        case ADD_EDGE: {
            if (!this->node_is_clicked) {
                this->clicked_node = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
                if (clicked_node) this->node_is_clicked = true;
            }
            else {
                std::shared_ptr<Node> dst = this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y);
                if (dst && dst != clicked_node) {
                    std::shared_ptr<Edge> to_add = std::make_shared<Edge>(this->clicked_node, dst);
                    this->graph.addEdge(to_add);
                }
                this->node_is_clicked = false;
                this->clicked_node = nullptr;
            }
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
            break;
        }
        case CHANGE_START_NODE: {
            this->graph.setStartNode(this->graph.getNodeByPosition(MOUSE_X, MOUSE_Y));
            break;
        }
        default: {
            break;
        }
    }

}


void Visualizer::runAlgorithm() {
    if (!this->graph.getStartNode()) return;
    vis_mode current_mode = this->mode;
    switch (current_mode) {
        case BFS: {
            if (!algo_thread_is_running) {
                this->window->setActive(false);
                algo_thread = std::thread(&Graph::runBFS, std::ref(this->graph), std::ref(*this->window),
                                          std::ref(this->toolbar));
            }
            break;
        }
        case DFS: {
            algo_thread = std::thread(&Graph::runBFS, std::ref(this->graph), std::ref(*this->window), std::ref(this->toolbar));
            algo_thread.join();
            break;
        }
        case MST: {
            algo_thread = std::thread(&Graph::runBFS, std::ref(this->graph), std::ref(*this->window), std::ref(this->toolbar));
            algo_thread.join();
            break;
        }
        case DIJKSTRA: {
            algo_thread = std::thread(&Graph::runBFS, std::ref(this->graph), std::ref(*this->window), std::ref(this->toolbar));
            algo_thread.join();
            break;
        }
    }
}


