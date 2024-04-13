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
  // Make sure initContextArea is called
  isInit = false;

  // SETUP VARIABLES
  frameCount = 0;
  prevTime = std::chrono::high_resolution_clock::now();
  elapsedFrames = 0;
  FPS = 0.0;

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
void ContextArea::calcFramesPerSecond() {
  // GET FPS
  auto now = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - prevTime);
  if (elapsed.count() >= 1) {     // 1 Second Elapsed
    prevTime = now;
    if(elapsedFrames != 0) {    // No Div by 0, end of the world!
      FPS = elapsedFrames / elapsed.count();
      elapsedFrames = 0;

      // DEBUG: Prints
      #ifdef ENABLE_DEBUG_PRINTS
        spdlog::info("FPS [%.2f]", FPS);
      #endif
    }
  }
  elapsedFrames++;
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

  // CALL VIRTUAL DRAW
  draw(ctx);

  // COUNTER TRACK
  calcFramesPerSecond();
  frameCount++;
  return isInit;          // Makes sure everything running smoothly
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
bool ContextArea::onKeyRelease(GdkEventKey *event) {
  if(event->keyval == GDK_KEY_space)
    printf("SPACE RELEASED\n");

  return true;
}

/**
 * Key Press Event
 * @param event - GDK Event Key
 */
bool ContextArea::onKeyPress(GdkEventKey *event) {
  if(event->keyval == GDK_KEY_space)
    printf("SPACE PRESSED\n");
  return true;
}

/**
 * Mouse Press Event
 * @param event - GDK Event Button
 */
bool ContextArea::onMousePress(GdkEventButton *event) {
    // RIGHT MOUSE CLICK!
  if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) )
    printf("MOUSE CLICK!\n");
	return true;
}


/* HELPER FUNCTIONS */

/**
 * Simple Wrapper for creating Image Buffer from Image Path
 * @param ctx - Cairo Drawing Context
 * @param path - Path to Image
 * @returns Image Reference Buffer
 */
GDK_IMAGE ContextArea::createImageBuffer(std::string path) {
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
GDK_IMAGE ContextArea::resizeImage(const GDK_IMAGE& img, int newWidth, int newHeight) {
  return img->scale_simple(newWidth, newHeight, Gdk::InterpType::INTERP_NEAREST);
}

/**
 * Draws Given Image from Path as large as the image
 *  is.
 *
 * @param ctx - Cario Drawing Context
 * @param path - Image Path
 */
void ContextArea::drawImage(const Context& ctx, GDK_IMAGE img) {
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
void ContextArea::circle(const Context& ctx, double x, double y, double r, RgbaColor color) {
  ctx.cairo_ctx->set_source_rgba(color.r, color.g, color.b, color.a);
  ctx.cairo_ctx->arc(x, y, r, 0, M_PI*2);
  ctx.cairo_ctx->fill();
}


/* SHARED FUNCTIONS */

void ContextArea::setup() {
  spdlog::info("Default Setup");
}

void ContextArea::draw(const Context& ctx) {}



/* PUBLIC FUNCTIONS */

/**
 * Initiates everything requried to run properly
 *  with FPS Target to 30 FPS
 */
void ContextArea::initContextArea() {
  // Functionality Should work Properly!
  isInit = true;

  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &ContextArea::on_timeout),
    34
  );

  // SETUP VIRTUAL FUNCTION
  setup();
}

/**
 * Initiates everything requried to run properly
 *  with Given FPS Target
 *
 * @param targetFPS - Enumerator for Targeted FPS
 */
void ContextArea::initContextArea(TARGET_FPS targetFPS) {
  // Functionality Should work Properly!
  isInit = true;

  // Figure out the Target FPS
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

  // SETUP VIRTUAL FUNCTION
  setup();
}


/**
 * @return Calculated Frames Per Second
 */
const double ContextArea::getFPS() {
  return FPS;
}

/**
 * Simple Wrapper for getting Mouse Position
 *
 * @param x - Reference to x-position of Mouse (Will be stored)
 * @param y - Reference to y-position of Mouse (Will be stored)
 */
void ContextArea::getMousePosition(double &x, double &y) {
  gdk_device_get_position_double(this->device, NULL, &x, &y);
}
