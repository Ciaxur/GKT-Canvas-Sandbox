#include "ContextArea.h"
#include "spdlog/spdlog.h"

// DEBUG: Debug Prints
// #define ENABLE_DEBUG_PRINTS


/* CONSTRUCTORS / DESTRUCTORS */

/**
 * Initiate ContextArea Object with Default Variables
 *  and Core Functionality
 */
ContextArea::ContextArea() {
  // Make sure init_context_area is called
  is_init = false;
  setup_called = false;

  // SETUP VARIABLES
  frame_count = 0;
  prev_time = std::chrono::high_resolution_clock::now();
  elapsed_frames = 0;
  fps = 0.0;

  // SETUP GDK DEVICE MANAGER
  display = gdk_display_get_default();
  seat = gdk_display_get_default_seat(display);
  device = gdk_seat_get_pointer(seat);
}

/**
 * Default Virtual Destructor
 */
ContextArea::~ContextArea() {}


/* PRIVATE CORE FUNCTIONS */

/**
 * Keeps Track / Calculates Frames Per Second
 */
void ContextArea::calc_frames_per_second() {
  // GET fps
  auto now = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - prev_time);
  if (elapsed.count() >= 1) {     // 1 Second Elapsed
    prev_time = now;
    if(elapsed_frames != 0) {    // No Div by 0, end of the world!
      fps = elapsed_frames / elapsed.count();
      elapsed_frames = 0;

      // DEBUG: Prints
      #ifdef ENABLE_DEBUG_PRINTS
        spdlog::info("fps [{:.2f}]", fps);
      #endif
    }
  }
  elapsed_frames++;
}



/**
 * On Draw Function for Draw Area
 *
 * @param ctx - Cairo Context
 */
bool ContextArea::on_draw(const CAIRO_CTX_REF& cairo_ctx) {
  // GET WINDOW DIMENSION DATA
  Gtk::Allocation allocation = get_allocation();
  const int WIDTH = allocation.get_width();
  const int HEIGHT = allocation.get_height();

  // CONSTRUCT CONTEXT
  const Context ctx{
    .cairo_ctx = cairo_ctx,
    .width = WIDTH,
    .height = HEIGHT,
  };

  // SETUP VIRTUAL FUNCTION
  if (!this->setup_called) {
    setup(ctx);
    this->setup_called = true;
  }

  // CALL VIRTUAL DRAW
  draw(ctx);

  // COUNTER TRACK
  calc_frames_per_second();
  frame_count++;
  return is_init;          // Makes sure everything running smoothly
}

/**
 * Forces Draw Area to Refresh
 */
bool ContextArea::on_timeout() {
  auto win = get_window();
  if(win)
    win->invalidate(false);
  return true;
}



/* EVENT FUNCTIONS */

/**
 * Key Release Event
 * @param event - GDK Event Key
 */
bool ContextArea::on_key_release(GdkEventKey *event) {
  if(event->keyval == GDK_KEY_space)
    spdlog::info("SPACE RELEASED");

  return true;
}

/**
 * Key Press Event
 * @param event - GDK Event Key
 */
bool ContextArea::on_key_press(GdkEventKey *event) {
  if(event->keyval == GDK_KEY_space)
    spdlog::info("SPACE PRESSED");
  return true;
}

/**
 * Mouse Press Event
 * @param event - GDK Event Button
 */
bool ContextArea::on_mouse_press(GdkEventButton *event) {
    // RIGHT MOUSE CLICK!
  if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) )
    spdlog::info("MOUSE CLICK!");
	return true;
}


/* HELPER FUNCTIONS */

/**
 * Simple Wrapper for creating Image Buffer from Image Path
 * @param ctx - Cairo Drawing Context
 * @param path - Path to Image
 * @returns Image Reference Buffer
 */
GDK_IMAGE ContextArea::create_image_buffer(std::string path) {
  return Gdk::Pixbuf::create_from_file(path);
}

/**
 * Simple, Easy wrapper for Resizing Image
 *
 * @param img - Reference to the Image that'll be resized
 * @param newWidth - New Width of Image
 * @param newHeight - New Height of Image
 * @return New GDK_IMAGE of Resized Image
 */
GDK_IMAGE ContextArea::resize_image(const GDK_IMAGE& img, int newWidth, int newHeight) {
  return img->scale_simple(newWidth, newHeight, Gdk::InterpType::INTERP_NEAREST);
}

/**
 * Draws Given Image from Path as large as the image
 *  is.
 *
 * @param ctx - Cario Drawing Context
 * @param path - Image Path
 */
void ContextArea::draw_image(const Context& ctx, GDK_IMAGE img) {
  Gdk::Cairo::set_source_pixbuf(ctx.cairo_ctx, img, 0, 0);
  ctx.cairo_ctx->rectangle(0, 0, img->get_width(), img->get_height());
  ctx.cairo_ctx->fill();
}

/**
 * Draws background color given the context and color to set background to.
 *
 * @param ctx - Drawing Context
 * @param color - RgbaColor struct.
 */
void ContextArea::background(const Context& ctx, RgbaColor color) {
  // Draw Background Color
  ctx.cairo_ctx->set_source_rgba(color.r, color.g, color.b, color.a);
  ctx.cairo_ctx->rectangle(0, 0, ctx.width, ctx.height);
  ctx.cairo_ctx->fill();
}

/**
 * Draws circle at given coordinates.
 *
 * @param ctx - Drawing Context
 * @param x - x-coordinate to draw circle at.
 * @param y - y-coordinate to draw circle at.
 * @param r - Circle's radius.
 * @param color - RgbaColor struct.
 */
void ContextArea::circle(const Context& ctx, double x, double y, double r, const RgbaColor &color) {
  set_color(ctx, color);
  ctx.cairo_ctx->arc(x, y, r, 0, M_PI*2);
  ctx.cairo_ctx->fill();
}

/**
 * Sets drawing context color.
 *
 * @param ctx - Drawing Context
 * @param color - RgbaColor struct.
 */
void ContextArea::set_color(const Context& ctx, const RgbaColor &color) {
  ctx.cairo_ctx->set_source_rgba(color.r, color.g, color.b, color.a);
}

/**
 * Sets drawing context text font size.
 *
 * @param ctx - Drawing Context
 * @param size - Font size
 */
void ContextArea::set_font_size(const Context& ctx, double size) {
  ctx.cairo_ctx->set_font_size(size);
}

/**
 * Draws text at the given coordinates.
 *
 * @param ctx - Drawing Context.
 * @param x - x-coordinate to draw text at.
 * @param y - y-coordinate to draw text at.
 * @param text - Text to draw.
 */
void ContextArea::draw_text(const Context& ctx, double x, double y, const char* text) {
  ctx.cairo_ctx->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
  ctx.cairo_ctx->move_to(x, y);
  ctx.cairo_ctx->show_text(text);
}

/**
 * Draws drawing statistics on the top right of the window.
 *  - fps
 *  - Window dimensions
 *
 * @param ctx - Drawing Context.
 */
void ContextArea::display_nerd_info(const Context& ctx) {
  const double font_size = 20.0;
  set_color(ctx, RgbaColor{ .r = 3.0, .a = 1.0 });
  set_font_size(ctx, font_size);

  // Store the extents of a text in order to grab the dimensions.
  Cairo::TextExtents f_extents;

  // DRAW fps COUNTER.
  // Interpret fps double as a string.
  char fps_buffer[255];
  int bytes_written = snprintf(fps_buffer, sizeof(fps_buffer), "fps: %.2f", this->get_fps());

  ctx.cairo_ctx->get_text_extents(fps_buffer, f_extents);
  draw_text(ctx, ctx.width - (f_extents.width + 5.f), font_size, fps_buffer);

  // DRAW WINDOW DIMENSIONS.
  char dim_buffer[255];
  bytes_written = snprintf(dim_buffer, sizeof(dim_buffer), "Window: width[%d] height[%d]", ctx.width, ctx.height);

  ctx.cairo_ctx->get_text_extents(dim_buffer, f_extents);
  draw_text(ctx, ctx.width - (f_extents.width + 5.f), font_size + font_size + 2.0, dim_buffer);
}


/* SHARED FUNCTIONS */

void ContextArea::setup(const Context& ctx) {
  spdlog::info("Default Setup");
}

void ContextArea::draw(const Context& ctx) {}



/* PUBLIC FUNCTIONS */

/**
 * Initiates everything requried to run properly
 *  with fps Target to 30 fps
 */
void ContextArea::init_context_area() {
  // Functionality Should work Properly!
  is_init = true;

  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &ContextArea::on_timeout),
    34
  );
}

/**
 * Initiates everything requried to run properly
 *  with Given fps Target
 *
 * @param targetFPS - Enumerator for Targeted fps
 */
void ContextArea::init_context_area(TARGET_FPS targetFPS) {
  // Functionality Should work Properly!
  is_init = true;

  // Figure out the Target fps
  unsigned int fps_interval;
  switch(targetFPS) {
    case FIFTEEN:
        fps_interval = 68;
        break;
    case THIRTY:
        fps_interval = 34;
        break;
    case SIXTY:
        fps_interval = 16;
        break;
    default:
        fps_interval = 34;
        break;
  }


  // Set "Draw Refresh Rate"
  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &ContextArea::on_timeout),
    fps_interval
  );
}


/**
 * @return Calculated Frames Per Second
 */
const double ContextArea::get_fps() {
  return fps;
}

/**
 * Simple Wrapper for getting Mouse Position
 *
 * @param x - Reference to x-position of Mouse (Will be stored)
 * @param y - Reference to y-position of Mouse (Will be stored)
 */
void ContextArea::get_mouse_position(double &x, double &y) {
  gdk_device_get_position_double(this->device, NULL, &x, &y);
}
