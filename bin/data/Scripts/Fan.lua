Fan = class()

function Fan:init(body)
    self.body = body
end

function Fan:frame(time)
	rayStart = self.body.transform.origin
	rayEnd = Vec3.new(0.0, 10.0, 0.0) + rayStart
	rayResult = physics:rayTest(rayStart, rayEnd, CollisionGroup.all, 1)
    if rayResult.hasHit then
        v1 = Vec3.new(0.0, 7.5, 0.0)
        v2 = (rayResult.hitPoint - rayResult.body.transform.origin)*0.5
        rayResult.body:applyForce(v1, v2)
    end
end

