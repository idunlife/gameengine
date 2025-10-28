// /c:/Users/DELL/Documents/personal/gameengine-1/moduarEngine/core/engine.cpp
// Self-contained engine implementation (module stubs + Engine).

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

// Simple module stubs. Replace with real implementations / headers.
struct Renderer {
    bool initialize() { std::cout << "[Renderer] Initialized\n"; return true; }
    void render() { /* draw frame */ }
    void shutdown() { std::cout << "[Renderer] Shutdown\n"; }
};

struct Physics {
    bool initialize() { std::cout << "[Physics] Initialized\n"; return true; }
    void update(double dt) { /* physics step using dt (seconds) */ }
    void shutdown() { std::cout << "[Physics] Shutdown\n"; }
};

struct Input {
    bool initialize() { std::cout << "[Input] Initialized\n"; return true; }
    void update() { /* poll / process input */ }
    void shutdown() { std::cout << "[Input] Shutdown\n"; }
};

struct Audio {
    bool initialize() { std::cout << "[Audio] Initialized\n"; return true; }
    void update() { /* update audio system */ }
    void shutdown() { std::cout << "[Audio] Shutdown\n"; }
};

// Engine class used by your main()
class Engine {
public:
    Engine()
        : isRunning(false)
    {}

    bool initialize() {
        // Initialize subsystems in order
        if (!input.initialize())      return failInit("Input");
        if (!renderer.initialize())   return failInit("Renderer");
        if (!physics.initialize())    return failInit("Physics");
        if (!audio.initialize())      return failInit("Audio");

        isRunning.store(true);
        std::cout << "Engine initialized\n";
        return true;
    }

    void run() {
        using clock = std::chrono::steady_clock;
        const double targetFPS = 60.0;
        const std::chrono::duration<double> timestep(1.0 / targetFPS);

        auto previous = clock::now();
        std::chrono::duration<double> accumulator(0.0);

        while (isRunning.load()) {
            auto now = clock::now();
            std::chrono::duration<double> frameTime = now - previous;
            previous = now;

            // clamp large frame times to avoid spiral of death
            if (frameTime.count() > 0.25) frameTime = std::chrono::duration<double>(0.25);

            accumulator += frameTime;

            // Handle input once per frame
            input.update();

            // Fixed-step physics updates
            while (accumulator >= timestep) {
                physics.update(timestep.count());
                accumulator -= timestep;
            }

            // Update other systems (audio, game logic etc.)
            audio.update();
            update();    // user/game logic hook

            // Render (as fast as loop runs, or consider vsync)
            renderer.render();

            // Sleep a bit to avoid busy loop (adjust as needed)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void shutdown() {
        // Shutdown subsystems in reverse order
        audio.shutdown();
        physics.shutdown();
        renderer.shutdown();
        input.shutdown();

        isRunning.store(false);
        std::cout << "Engine shutdown\n";
    }

    void stop() { isRunning.store(false); }

private:
    std::atomic<bool> isRunning;

    // Module instances
    Renderer renderer;
    Physics physics;
    Input input;
    Audio audio;

    // Hook for game-specific updates (empty here)
    void update() {
        // Game logic would go here.
    }

    bool failInit(const char* moduleName) {
        std::cerr << "Failed to initialize " << moduleName << "\n";
        shutdown();
        return false;
    }
};