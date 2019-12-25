#pragma once
#include "./ContextArea.h"

/**
 * Simple Window Class that Handles GTK
 *  Window Creation as well as Keyboard Events
 */
class MyWindow : public Gtk::Window {
    private:
        ContextArea *drawArea;      // Pointer to ContextArea Object
        bool handled;               // If drawArea is Handled by Current Object
    
    private:
        /**
         * Initiates Common Properties
         */
        void initProperties();
    
    public:
        /**
         * Create Default Window
         */
        MyWindow();

        /**
         * Customized Window
         * 
         * @param _drawArea - Reference to Created ContextArea Object
         * @param width - Window Width
         * @param height - Window Height
         * @param title - Window Title (Default = 123FLOAT)
         */
        MyWindow(ContextArea& _drawArea, int width = 640, int height = 480, std::string title = "123FLOAT");

        /**
         * Virtual Constructor
         *  Defaulted to Free up Context Area Space
         */
        virtual ~MyWindow();

    private:        // Key Event Callbacks
        bool on_key_press_event(GdkEventKey *event);                    // Key Press Event
        bool on_key_release_event(GdkEventKey *event);                  // Key Release Event
        bool on_button_press_event(GdkEventButton *event);              // Button Press Event
};
