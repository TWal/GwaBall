FramePrinter = class()

function FramePrinter:init()
end

function FramePrinter:frame(time)
    print("FPS: " .. 1/time)
end

