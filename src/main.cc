// CORE CLASSES
#include "MyWindow.h"
#include "spdlog/spdlog.h"

// MATHS
#define _USE_MATH_DEFINES
#include <math.h>

struct Vector2D {
  double x;
  double y;
};

struct Body {
  // Coordinates.
  Vector2D loc;

  // TODO: implement better bounding.
  double radius;

  // Physics!
  Vector2D velocity;
  Vector2D acceleration;
};


/**
 * Simple Example for Sandbox Use :)
 */
class MyApp: public ContextArea {
  public:
    MyApp() {
      spdlog::info("MyApp Constructed");
      init_context_area(TARGET_FPS::SIXTY);
    }


  private:    // KEYBOARD EVENTS
    bool on_key_press(GdkEventKey* event) {
      if(event->keyval == GDK_KEY_q) {        // Quit on Key Press 'Q'
        spdlog::info("Quitting out of Application!");
        return false;
      }

      // Return True to keep Running
      return true;
    }

    bool on_key_release(GdkEventKey* event) {     // Similair to KeyPress
      // Return True to keep Running
      return true;
    }

    bool on_mouse_press(GdkEventButton* event) {
      return true;
    }

  private:    // DRAWING FUNCTIONS
    Vector2D gravity{
      .x = 0.f,
      .y = 0.1f,
    };
    Body circle_body1;

    void setup(const Context& ctx) {
      spdlog::info("SETTING UP...");

      this->circle_body1 = Body {
        // Intiial position.
        .loc = Vector2D{
          .x = ctx.width / 2.f,
          .y = ctx.height / 2.f,
        },
        .radius = 20.f,

        // Initial force.
        .velocity = Vector2D{
          .x = 0.f,
          .y = 0.f,
        },
        .acceleration = Vector2D{
          .x = 5.f,
          .y = 0.f,
        },
      };
    }

    void update_physics(const Context& ctx, Body *body) {
      // Apply gravity.
      body->acceleration.y += gravity.y;

      // Apply velocity.
      body->velocity.x += body->acceleration.x;
      body->velocity.y += body->acceleration.y;

      // Apply displacement.
      body->loc.x += body->velocity.x;
      body->loc.y += body->velocity.y;

      // Apply bounce when contacting an edge.
      // Bottom edge.
      if (body->loc.y + circle_body1.radius>= ctx.height)
        body->acceleration.y *= -1.f;

      // Top edge.
      if (body->loc.y - circle_body1.radius <= 0)
        body->acceleration.y *= -1.f;

      // Left edge.
      if (body->loc.x - circle_body1.radius <= 0)
        body->acceleration.x *= -1.f;

      // Right edge.
      if (body->loc.x + circle_body1.radius >= ctx.width)
        body->acceleration.x *= -1.f;

      // Reset velocity.
      body->velocity.x = 0;
      body->velocity.y = 0;
    }

    void draw(const Context& ctx) {
      // Static color definitions.
      static const RgbaColor RED{
        .r = 1.0,
        .g = 0.0,
        .b = 0.0,
        .a = 1.0,
      };
      static const RgbaColor BLUE{
        .r = 0.0,
        .g = 0.0,
        .b = 1.0,
        .a = 1.0,
      };


      // Draw Background Color
      background(ctx, RgbaColor{
        .r = 3.0,
        .g = 3.0,
        .b = 3.0,
        .a = 1.0,
      });

      // Draw nerd info at the top right.
      display_nerd_info(ctx);

      // Draw Circle Traveling Across Screen
      circle(ctx, circle_body1.loc.x, circle_body1.loc.y, circle_body1.radius, RED);

      // Update the physics on bodies.
      update_physics(ctx, &circle_body1);
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
