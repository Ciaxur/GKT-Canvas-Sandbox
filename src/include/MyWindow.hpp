#include "./ContextArea.h"

class MyWindow : public Gtk::Window {
    private:
        ContextArea *drawArea;      // Pointer to ContextArea Object
        bool handled;               // If drawArea is Handled by Current Object
    
    private:
        /**
         * Initiates Common Properties
         */
        void initProperties() {
            // INIT DRAW AREA
            add(*drawArea);
            drawArea->show();

            // SETUP EVENTS
            Gtk::Window::add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
        }
    
    public:
        /**
         * Create Default Window
         */
        MyWindow() {
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
        MyWindow(ContextArea& _drawArea, int width = 640, int height = 480, std::string title = "123FLOAT") {
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
        virtual ~MyWindow() {
            if(drawArea && handled)
                delete drawArea;
        }

    private:        // Key Event Callbacks
        bool on_key_press_event(GdkEventKey *event) {
            if(!drawArea->onKeyPress(event)) this->destroy_();
            return true;
        }

        bool on_key_release_event(GdkEventKey *event) {
            if(!drawArea->onKeyRelease(event)) this->destroy_();
            return true;
        }
};