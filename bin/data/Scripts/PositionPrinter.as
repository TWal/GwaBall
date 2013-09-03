class PositionPrinter : Script {
    RigidBody@ _body;
    PositionPrinter(RigidBody@ body) {
        @_body = @body;
    }
    void frame(double time) {
        Vec3 pos = _body.transform.pos;
        print("(" + pos.x + ", " + pos.y + ", " + pos.z + ")\n");
    }
}

