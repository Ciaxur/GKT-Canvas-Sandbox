// CORE CLASSES
#include "MyWindow.h"
#include "spdlog/spdlog.h"

// MATHS
#define _USE_MATH_DEFINES
#include <math.h>


/**
 * Simple Example for Sandbox Use :)
 */
class MyApp: public ContextArea {
  public:
    MyApp() {
      spdlog::info("MyApp Constructed");
      initContextArea();
    }


  private:    // KEYBOARD EVENTS
    bool onKeyPress(GdkEventKey* event) {
      if(event->keyval == GDK_KEY_q) {        // Quit on Key Press 'Q'
        spdlog::info("Quitting out of Application!");
        return false;
      }

      // Return True to keep Running
      return true;
    }

    bool onKeyRelease(GdkEventKey* event) {     // Similair to KeyPress
      // Return True to keep Running
      return true;
    }

    bool onMousePress(GdkEventButton* event) {
      return true;
    }

  private:    // DRAWING FUNCTIONS
    int xc = 0;

    void setup() {
      spdlog::info("SETTING UP...");
    }

    void draw(const Context& ctx) {
      // Draw Background Color
      background(ctx, RgbaColor{
        .r = 3.0,
        .g = 3.0,
        .b = 3.0,
        .a = 1.0,
      });

      // Draw Circle Traveling Across Sceen
      circle(ctx, xc, ctx.height/2, 20, RgbaColor{
        .r = 1.0,
        .g = 0.0,
        .b = 0.0,
        .a = 1.0,
      });

      // Increment xc Till the Edge
      xc = (xc + 2) % ctx.width;
    }
};



int main(int argc, char *argv[]) {
  // START WINDOW + APP
  auto gtk_application = Gtk::Application::create(argc, argv, "org.gtkmm.sandbox.base");
  MyApp my_app;
  MyWindow window(my_app, 200, 200, "GTK Canvas Sandbox");

  // Run the Application
  return gtk_application->run(window);
}
