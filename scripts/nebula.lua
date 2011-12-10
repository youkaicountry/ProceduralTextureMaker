--by Barebones

function setup ()

base = newTexture (256, 256)
shape = newTexture (256, 256)

makeFinal (base)

fractionalNoise (base, -1, 0, 0, 0, 128, 128, 128)

perlinNoise (shape, 0, 0, 30, 0.3, 3, -1)
quantize (shape, 2, 2, 2)
tint (shape, 128, 64, 0)

for i = 1, 10 do
    applyScale (shape, math.random (0.8, 1.2), math.random (0.8, 1.2), 0, 0, 0)
    applyRotationAbout (shape, math.random (5, 20), math.random (255), math.random (255), 0, 0, 0)
    add (base, shape, 2)
    applyBlur (base, -2, 5, 1, 1, 1)
end

end
