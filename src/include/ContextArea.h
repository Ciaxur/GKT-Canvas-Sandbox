#pragma once

// Library Includes
#include <gtkmm.h>
#include <chrono>
#include <iostream>

// BETTER READABILITY
#define GDK_IMAGE Glib::RefPtr<Gdk::Pixbuf>
#define CTX_REF Cairo::RefPtr<Cairo::Context>
#define CHRONO_HIGH_RES_CLOCK std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds>

// DEBUG: Debug Prints
#define ENABLE_DEBUG_PRINTS 0


/**
 * Enumeration for Targeted Frames Per Second
 */
enum TARGET_FPS {
    SIXTY, THIRTY, FIFTEEN
};


/**
 * 
 * REQUIRED FUNCTIONS
 *  - initContextArea
 *      - Must Be called prior to running
 * 
 * MACRO DEFINITIONS
 *  - "ENABLE_DEBUG_PRINTS" (True/False)
 *      - Enables Debug Prints
 */
class ContextArea: public Gtk::DrawingArea {
    protected:      // Shared Variables
        unsigned long long      frameCount;                         // Keep Track of Frames
        double                  FPS;                                // Total Calulated Frames per Second
        
    private:        // Private Core Variables
        CHRONO_HIGH_RES_CLOCK   prevTime;                           // Previous Time since Frame
        int                     elapsedFrames;                      // Counter for Frames Drawn
        bool                    isInit;                             // Initiated Status, If initContextArea is Called
    
    private:        // Private Core Functions
        void calcFramesPerSecond();                                 // Calculates Frames Per Second
        bool on_draw(const CTX_REF&) override;                      // Called by GTK
        bool on_timeout();                                          // Timer for Re-Draw

    public:      // Event Functions
        virtual bool onKeyRelease(GdkEventKey*);                    // Key Release Event
        virtual bool onKeyPress(GdkEventKey*);                      // Key Press Event
    
    protected:      // Helper Functions
        GDK_IMAGE createImageBuffer(std::string);                   // Easy Wrapper for Image Buffer
        GDK_IMAGE resizeImage(const GDK_IMAGE&, int, int);          // Easy Wrapper for Resizing given Image
        void drawImage(const CTX_REF&, GDK_IMAGE);                  // Draws Given Image at (0,0)
        

    protected:      // Shared Functions
        virtual void setup();                                       // Called Prior to Draw
        virtual void draw(const CTX_REF&, const int, const int);    // Easy to use Shared Draw Function

        
    public:         // Public Functions
        void initContextArea();                                     // Must Be Called Prior to Running
        void initContextArea(TARGET_FPS);                           // Must Be Called Prior to Running With Given FPS Target
        const double getFPS();                                      // Returns Current FPS
    

    public:         // Constructor/Destructor
        ContextArea();
        virtual ~ContextArea();
};