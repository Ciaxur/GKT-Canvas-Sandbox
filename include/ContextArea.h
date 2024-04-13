#pragma once

// Library Includes
#include <gtkmm.h>
#include <chrono>
#include <iostream>

// BETTER READABILITY
#define GDK_IMAGE Glib::RefPtr<Gdk::Pixbuf>
#define CAIRO_CTX_REF Cairo::RefPtr<Cairo::Context>
#define CHRONO_HIGH_RES_CLOCK std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds>

/**
 * Enumeration for Targeted Frames Per Second
 */
enum TARGET_FPS {
  SIXTY, THIRTY, FIFTEEN
};

/**
 * Color Struct.
 */
struct RgbaColor {
  double r;
  double g;
  double b;
  double a;
};

/**
 * Context struct which is a thin-wrapper that includes the cairo and ContextArea
 * metadata being passed around.
*/
struct Context {
  const CAIRO_CTX_REF &cairo_ctx;
  const int width;
  const int height;
};

/**
 *
 * REQUIRED FUNCTIONS
 *  - init_context_area
 *      - Must Be called prior to running
 *
 * MACRO DEFINITIONS
 *  - "ENABLE_DEBUG_PRINTS" (True/False)
 *      - Enables Debug Prints
 */
class ContextArea: public Gtk::DrawingArea {
  protected:      // Shared Variables
    unsigned long long      frame_count;                        // Keep Track of Frames
    double                  fps;                                // Total Calulated Frames per Second

  private:        // Private Core Variables
    CHRONO_HIGH_RES_CLOCK   prev_time;                          // Previous Time since Frame
    int                     elapsed_frames;                     // Counter for Frames Drawn
    bool                    is_init;                            // Initiated Status, If init_context_area is Called
    bool                    setup_called;                       // State of setup being invoked.

    // GDK Variables
    GdkDisplay              *display;
    GdkSeat                 *seat;
    GdkDevice               *device;

  private:        // Private Core Functions
    void calc_frames_per_second();                              // Calculates Frames Per Second
    bool on_draw(const CAIRO_CTX_REF&) override;                // Called by GTK
    bool on_timeout();                                          // Timer for Re-Draw

  public:      // Event Functions
    virtual bool on_key_release(GdkEventKey*);                  // Key Release Event
    virtual bool on_key_press(GdkEventKey*);                    // Key Press Event
    virtual bool on_mouse_press(GdkEventButton*);               // Mouse Press Event

  protected:      // Helper Functions
    GDK_IMAGE create_image_buffer(std::string);                 // Easy Wrapper for Image Buffer
    GDK_IMAGE resize_image(const GDK_IMAGE&, int, int);         // Easy Wrapper for Resizing given Image
    void draw_image(const Context&, GDK_IMAGE);                 // Draws Given Image at (0,0)
    void background(const Context&, RgbaColor);                 // Draws background color.

    // Draws a circle at given coordinates.
    void circle(const Context&, double x, double y, double r, RgbaColor);

    // Sets drawing context color.
    void set_color(const Context&, RgbaColor);

    // Sets drawing context text font size.
    void set_font_size(const Context&, double);

    // Draws text at the given coordinates.
    void draw_text(const Context&, double x, double y, const char*);

    // Draws information such as fps at the top right.
    void display_nerd_info(const Context&);


  protected:      // Shared Functions
    virtual void setup(const Context&);                         // Called ONCE prior to Draw function
    virtual void draw(const Context&);                          // Easy to use Shared Draw function


  public:         // Public Functions
    void init_context_area();                                   // Must Be Called Prior to Running
    void init_context_area(TARGET_FPS);                         // Must Be Called Prior to Running With Given fps Target
    const double get_fps();                                     // Returns Current fps
    void get_mouse_position(double &x, double &y);              // Simple Wrapper for Getting Mouse Position

  public:         // Constructor/Destructor
    ContextArea();
    virtual ~ContextArea();
};