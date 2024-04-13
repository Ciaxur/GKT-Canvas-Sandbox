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
static const RgbaColor WHITE{
  .r = 0.0,
  .g = 0.0,
  .b = 0.0,
  .a = 1.0,
};
static const RgbaColor BACKGROUND_COLOR{
  .r = 3.0,
  .g = 3.0,
  .b = 3.0,
  .a = 1.0,
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
      if (body->loc.y + body->radius>= ctx.height) {
        body->acceleration.y *= -1.f;
        body->loc.y = ctx.height - body->radius - 1.f;
      }

      // Top edge.
      if (body->loc.y - body->radius <= 0) {
        body->acceleration.y *= -1.f;
        body->loc.y = body->radius + 1.0f;
      }

      // Left edge.
      if (body->loc.x - body->radius <= 0) {
        body->acceleration.x *= -1.f;
        body->loc.x = body->radius + 1.0f;
      }

      // Right edge.
      if (body->loc.x + body->radius >= ctx.width) {
        body->acceleration.x *= -1.f;
        body->loc.x = ctx.width - body->radius - 1.f;
      }

      // STATS/DEBUG: //
      double text_offset = 30.f;


      // Draw the circle body's state stats.
      set_font_size(ctx, 25.f);
      char body_d_stat_buffer[255];
      snprintf(body_d_stat_buffer, sizeof(body_d_stat_buffer), "d[x=%.2f|y=%2.f]", circle_body1.loc.x, circle_body1.loc.y);
      draw_text(
        ctx,
        circle_body1.loc.x,
        circle_body1.loc.y - text_offset,
        body_d_stat_buffer
      );

      char body_a_stat_buffer[255];
      snprintf(body_a_stat_buffer, sizeof(body_a_stat_buffer), "a[x=%.2f|y=%2.f]", circle_body1.acceleration.x, circle_body1.acceleration.y);
      draw_text(
        ctx,
        circle_body1.loc.x,
        circle_body1.loc.y - (text_offset * 2.f),
        body_a_stat_buffer
      );


      // Draw direction of force.
      double magnitude = std::sqrt(body->velocity.x * body->velocity.x + body->velocity.y * body->velocity.y);
      double direction_rad = std::atan2(body->velocity.y, body->velocity.x);
      double direction_deg = direction_rad * 180.f / M_PI;

      draw_line(
        ctx,
        body->loc,
        Vector2D{
          .x = body->loc.x + magnitude * std::cos(direction_rad),
          .y = body->loc.y + magnitude * std::sin(direction_rad),
        }
      );

      char body_mag_buffer[255];
      snprintf(body_mag_buffer, sizeof(body_mag_buffer), "mag=%.2f | direction=%.2frad", magnitude, direction_rad);
      draw_text(
        ctx,
        circle_body1.loc.x,
        circle_body1.loc.y - (text_offset * 3.f),
        body_mag_buffer
      );

      // Reset velocity.
      body->velocity.x = 0;
      body->velocity.y = 0;
    }

    void draw_line(const Context& ctx, Vector2D pos1, Vector2D pos2) {
      set_color(ctx, BLUE);
      ctx.cairo_ctx->set_line_width(10.f);
      ctx.cairo_ctx->set_line_cap(Cairo::LINE_CAP_ROUND);
      ctx.cairo_ctx->move_to(pos1.x, pos1.y);
      ctx.cairo_ctx->line_to(pos2.x, pos2.y);
      ctx.cairo_ctx->stroke();
    }

    void draw(const Context& ctx) {
      // Draw Background Color
      background(ctx, BACKGROUND_COLOR);

      // Draw nerd info at the top right.
      display_nerd_info(ctx);

      // Draw circle bouncing across the screen.
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
