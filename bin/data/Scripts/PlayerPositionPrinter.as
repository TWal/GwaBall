class PlayerPositionPrinter : Script {
    void frame(double time) {
        Vec3 pos = game.player.body.transform.pos;
        print("Player position: (" + pos.x + ", " + pos.y + ", " + pos.z + ")\n");
    }
}
