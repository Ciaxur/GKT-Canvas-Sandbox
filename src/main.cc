// CORE CLASSES
#include "MyWindow.h"
#include "spdlog/spdlog.h"

// MATHS
#define _USE_MATH_DEFINES
#include <math.h>
#include <deque>

const double GRAVITATIONAL_CONST = 1.f;

struct Vector2D {
  double x;
  double y;
};

struct Body {
  // Coordinates.
  Vector2D pos;

  RgbaColor color;

  // TODO: implement better bounding.
  double radius;

  // Physics!
  double mass;
  Vector2D velocity;
  Vector2D acceleration;

  std::deque<Vector2D> trail;
  size_t max_trail_size;
};

// Static color definitions.
static const RgbaColor RED{
  .r = 1.0,
  .g = 0.0,
  .b = 0.0,
  .a = 1.0,
};
static const RgbaColor GREEN{
  .r = 0.0,
  .g = 1.0,
  .b = 0.0,
  .a = 1.0,
};
static const RgbaColor BLUE{
  .r = 0.0,
  .g = 0.0,
  .b = 1.0,
  .a = 1.0,
};
static const RgbaColor CYAN{
  .r = 0.0,
  .g = 1.0,
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
    std::vector<Body> bodies;

    void setup(const Context& ctx) {
      spdlog::info("SETTING UP...");

      this->bodies.push_back({
        // Intiial position.
        .pos = Vector2D{
          .x = ctx.width / 2.f,
          .y = ctx.height / 2.f,
        },
        .color = RED,
        .radius = 20.f,

        // Initial force.
        .mass = 500.f,
        .velocity = Vector2D{
          .x = 0.f,
          .y = 0.f,
        },
        .acceleration = Vector2D{
          .x = 0.f,
          .y = 0.f,
        },
        .max_trail_size = 32,
      });

      this->bodies.push_back({
        // Intiial position.
        .pos = Vector2D{
          .x = (ctx.width / 2.f) + (20.f * 4.f),
          .y = ctx.height / 2.f,
        },
        .color = BLUE,
        .radius = 20.f,

        // Initial force.
        .mass = 10.f,
        .velocity = Vector2D{
          .x = 0.f,
          .y = 0.f,
        },
        .acceleration = Vector2D{
          .x = 0.f,
          .y = 2.5f,
        },
        .max_trail_size = 32,
      });
    }

    // Transfers energy from one body to another.
    void transfer_energy(Body *b1, Body *b2) {
      // Conservation of momentum!
      double total_mass = b1->mass + b2->mass;

      Vector2D b1_v{
        ( 2.f * b2->mass * b2->velocity.x + (b1->mass - b2->mass) * b1->velocity.x) / total_mass,
        ( 2.f * b2->mass * b2->velocity.y + (b1->mass - b2->mass) * b1->velocity.y) / total_mass,
      };
      Vector2D b2_v{
        ( 2.f * b1->mass * b1->velocity.x + (b2->mass - b1->mass) * b2->velocity.x) / total_mass,
        ( 2.f * b1->mass * b1->velocity.y + (b2->mass - b1->mass) * b2->velocity.y) / total_mass,
      };

      b1->velocity = b1_v;
      b2->velocity = b2_v;
    }

    double distance(const Body &body1, const Body &body2) {
      return std::sqrt(
        std::pow(body2.pos.x - body1.pos.x, 2) + std::pow(body2.pos.y - body1.pos.y, 2)
      );

    }

    Vector2D midpoint(const Body &b1, const Body &b2) {
      double dx = b2.pos.x - b1.pos.x;
      double dy = b2.pos.y - b1.pos.y;
      return {
        b1.pos.x + (dx / 2.f),
        b1.pos.y + (dy / 2.f)
      };
    }

    bool is_collide(const Body &b1, const Body &b2) {
      double d = distance(b1, b2);
      return b1.radius + b2.radius > d;
    }

    void unlodge_bodies(Body *b1, Body *b2) {
      double d = distance(*b1, *b2);
      double dx = (b2->pos.x - b1->pos.x) / d;
      double dy = (b2->pos.y - b1->pos.y) / d;

      b1->pos.x -= dx;
      b1->pos.y -= dy;
    }

    // Calculates the force exertered on body1 from body2.
    Vector2D calculate_force_on_body(const Body &body1, const Body &body2) {
      double r = distance(body1, body2);
      double f_mag = GRAVITATIONAL_CONST * ( (body1.mass * body2.mass) / std::pow(r, 2) );
      double angle = std::atan2( body2.pos.y - body1.pos.y, body2.pos.x - body1.pos.x );
      return { f_mag * std::cos(angle), f_mag * std::sin(angle) };
    }

    void draw_force_on_body(const Context &ctx, const Body &b1, Vector2D force) {
      // Magical multiplier to so we can see the force arrow.
      Vector2D p1{
        b1.pos.x + force.x / b1.mass,
        b1.pos.y + force.y / b1.mass,
      };

      draw_line(
        ctx,
        b1.pos,
        p1,
        GREEN
      );
    }

    void update_physics(const Context &ctx, std::vector<Body> &bodies) {
      for (size_t i = 0; i < bodies.size(); i++) {
        Body *body = &bodies[i];

        // Calculate the gravitational force between all bodies.
        for (size_t j = 0; j < bodies.size(); j++) {
          // Ignore self.
          if (i == j) continue;
          Body other_body = bodies[j];

          Vector2D force = calculate_force_on_body(*body, other_body);
          draw_force_on_body(ctx, *body, force);

          if (is_collide(*body, other_body)) {
            transfer_energy(body, &other_body);
            unlodge_bodies(body, &other_body);
          }

          // Update acceleration on each other.
          body->acceleration.x += force.x / body->mass;
          body->acceleration.y += force.y / body->mass;
        }

        // Update velocity and displacement.
        body->velocity.x += body->acceleration.x;
        body->velocity.y += body->acceleration.y;

        body->pos.x += body->velocity.x;
        body->pos.y += body->velocity.y;

        // Reset acceleration.
        body->acceleration.x = 0.f;
        body->acceleration.y = 0.f;

        draw_body_stats(ctx, *body);
      }

    }

    void draw_body_stats(const Context &ctx, const Body &body) {
      // STATS/DEBUG: //
      double text_offset = 18.f;
      double font_size = 12.f;

      // Draw the circle body's state stats.
      set_color(ctx, RED);
      set_font_size(ctx, font_size);
      char body_d_stat_buffer[255];
      snprintf(body_d_stat_buffer, sizeof(body_d_stat_buffer), "d[x=%.2f|y=%2.f]", body.pos.x, body.pos.y);
      draw_text(
        ctx,
        body.pos.x,
        body.pos.y - text_offset,
        body_d_stat_buffer
      );

      char body_a_stat_buffer[255];
      snprintf(body_a_stat_buffer, sizeof(body_a_stat_buffer), "a[x=%.2f|y=%2.f]", body.acceleration.x, body.acceleration.y);
      draw_text(
        ctx,
        body.pos.x,
        body.pos.y - (text_offset * 2.f),
        body_a_stat_buffer
      );


      // Draw direction of force.
      double magnitude = std::sqrt(body.velocity.x * body.velocity.x + body.velocity.y * body.velocity.y);
      double direction_rad = std::atan2(body.velocity.y, body.velocity.x);
      double direction_deg = direction_rad * 180.f / M_PI;

      draw_line(
        ctx,
        body.pos,
        Vector2D{
          .x = body.pos.x + magnitude * std::cos(direction_rad),
          .y = body.pos.y + magnitude * std::sin(direction_rad),
        },
        RED
      );

      char body_mag_buffer[255];
      snprintf(body_mag_buffer, sizeof(body_mag_buffer), "mag=%.2f | direction=%.2frad", magnitude, direction_rad);
      draw_text(
        ctx,
        body.pos.x,
        body.pos.y - (text_offset * 3.f),
        body_mag_buffer
      );
    }

    void draw_line(const Context& ctx, Vector2D pos1, Vector2D pos2, RgbaColor color) {
      set_color(ctx, color);
      ctx.cairo_ctx->set_line_width(10.f);
      ctx.cairo_ctx->set_line_cap(Cairo::LINE_CAP_ROUND);
      ctx.cairo_ctx->move_to(pos1.x, pos1.y);
      ctx.cairo_ctx->line_to(pos2.x, pos2.y);
      ctx.cairo_ctx->stroke();
    }

    void draw_body_on_mouse(const Context& ctx, Body *body) {
      this->get_mouse_position(body->pos.x, body->pos.y);
    }

    void draw(const Context& ctx) {
      // Draw Background Color
      background(ctx, BACKGROUND_COLOR);

      // Draw nerd info at the top right.
      display_nerd_info(ctx);

      // Draw them bodies.
      for (Body &body : this->bodies) {
        // Draw trail.
        for (size_t i = 0; i < body.trail.size(); i++) {
          const Vector2D &trail = body.trail[i];
          RgbaColor color = CYAN;

          // Normalized change in trail alpha mapped to the number of max trails.
          float trail_off_alpha_dt = 1.f - ((i - 0.f) / (body.max_trail_size - 0.f));
          color.a = trail_off_alpha_dt;

          circle(ctx, trail.x, trail.y, body.radius / 2.f, color);
        }

        // Track trail.
        if (body.trail.size() >= body.max_trail_size)
          body.trail.pop_front();

        // Copy the current state of the trail.
        body.trail.push_back(body.pos);

        circle(ctx, body.pos.x, body.pos.y, body.radius, body.color);
      }

      // DEBUG:
      // draw_body_on_mouse(ctx, &this->bodies[0]);

      // Update the physics on bodies.
      update_physics(ctx, bodies);
    }
};



int main(int argc, char *argv[]) {
  // START WINDOW + APP
  auto gtk_application = Gtk::Application::create(argc, argv, "org.gtkmm.sandbox.base");
  MyApp my_app;
  MyWindow window(my_app, 800, 800, "GTK Canvas Sandbox");

  // Run the Application
  return gtk_application->run(window);
}
