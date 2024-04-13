#include "MyWindow.h"


/* SETUP METHODS */

/**
 * Initiates Common Properties
 */
void MyWindow::initProperties() {
  // INIT DRAW AREA
  add(*drawArea);
  drawArea->show();

  // SETUP EVENTS
  Gtk::Window::add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK);
}


/* CONSTRUCTORS & DESTRUCTORS */

/**
 * Create Default Window
 */
MyWindow::MyWindow() {
  // Setup Properties
  set_default_size(640, 480);
  set_title("123FLOAT");

  // Create and Handle ContextArea
  drawArea = new ContextArea();
  handled = true;

  // Initiate Window Properties
  initProperties();
}

/**
 * Customized Window
 *
 * @param _drawArea - Reference to Created ContextArea Object
 * @param width - Window Width
 * @param height - Window Height
 * @param title - Window Title
 */
MyWindow::MyWindow(ContextArea& _drawArea, int width, int height, std::string title) {
  // Setup Window Properties
  set_default_size(width, height);
  set_title(title);

  // Assign ContextArea
  drawArea = &_drawArea;
  handled = false;

  // Intiate Window Properties
  initProperties();
}

/**
 * Virtual Constructor
 *  Defaulted to Free up Context Area Space
 */
MyWindow::~MyWindow() {
  if(drawArea && handled)
    delete drawArea;
}


/* KEYBOARD EVENT CALLBACKS */

bool MyWindow::on_key_press_event(GdkEventKey *event) {
  if(!drawArea->onKeyPress(event)) this->destroy_();
  return true;
}

bool MyWindow::on_key_release_event(GdkEventKey *event) {
  if(!drawArea->onKeyRelease(event)) this->destroy_();
  return true;
}

/* MOUSE EVENT CALLBACKS */
bool MyWindow::on_button_press_event(GdkEventButton *event) {
  if(!drawArea->onMousePress(event)) this->destroy_();
	return true;
}
