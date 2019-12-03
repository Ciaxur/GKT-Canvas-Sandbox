// CORE CLASSES
#include "include/MyWindow.h"

// MATHS
#define _USE_MATH_DEFINES
#include <math.h>


/**
 * Simple Example for Sandbox Use :)
 */
class MyApp: public ContextArea {
    public:
        MyApp() {
            std::cout << "MyApp Constructed\n";
            initContextArea();
        }
    

    private:    // KEYBOARD EVENTS
        bool onKeyPress(GdkEventKey* event) {
            if(event->keyval == GDK_KEY_q) {        // Quit on Key Press 'Q'
                std::cout << "Quitting out of Application!" << std::endl;
                return false;
            }
            
            // Return True to keep Running
            return true;
        }

        bool onKeyRelease(GdkEventKey* event) {     // Similair to KeyPress
            // Return True to keep Running
            return true;
        }

        

    private:    // DRAWING FUNCTIONS
        int xc = 0;
    
        void setup() {
            std::cout << "SETTING UP...\n";
        }

        void draw(const CTX_REF& ctx, const int WIDTH, const int HEIGHT) {
            // Draw Background Color
            ctx->set_source_rgb(3.0, 3.0, 3.0);
            ctx->rectangle(0, 0, WIDTH, HEIGHT);
            ctx->fill();

            // Draw Circle Traveling Across Sceen
            ctx->set_source_rgb(1.0, 0.0, 0.0);
            ctx->arc(xc, HEIGHT/2, 20, 0, M_PI*2);
            ctx->fill();

            // Increment xc Till the Edge
            xc = (xc + 2) % WIDTH;
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
