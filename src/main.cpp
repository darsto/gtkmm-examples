/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#include "Application.h"
#include "thread/Core.h"

int main(int argc, char **argv) {
    Application app(argc, argv);

    Core core(app);
    app.bindCore(&core);
    
    for (int attempts = 0; !core.running(); ++attempts) { //wait for the core to initialize (on separate thread)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (attempts > 20) {
            app.showErrorWindow("Couldn't initialize core thread");
        }
    }

    auto ret = app.run();
    core.stop();

    return ret;
}