class FramePrinter : Script {
    void frame(double time) {
        print("FPS: " + 1/time + "\n");
    }
}
