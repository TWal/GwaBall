class PositionPrinter : Script {
    RigidBody@ _body;
    PositionPrinter(RigidBody@ body) {
        @_body = @body;
    }
    void frame(double time) {
        Vector3 origin = _body.transform.origin;
        print("(" + origin.x + ", " + origin.y + ", " + origin.z + ")\n");
    }
}

