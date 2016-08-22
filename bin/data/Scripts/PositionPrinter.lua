PositionPrinter = class()

function PositionPrinter:init(body, id)
    self.body = body
    self.id = id
end

function PositionPrinter:frame(time)
    vec = self.body.transform.origin
    print(self.id .. ": " .. vec.x .. " " .. vec.y .. " " .. vec.z)
end

