--by Barebones

function setup ()

base = newTexture (256, 256)
bump = newTexture (256, 256)

makeFinal (base)

fractionalNoise (base, -1, 0, 0, 0, 256, 176, 128)
applyBlur (base, 0, 3, 1, 1, 1)

perlinNoise (bump, 0, 0, 40, 0.3, 3, -1)
quantize (bump, 2, 2, 2)
applyBlur (bump, 0, 30, 1, 1, 1)

light (base, bump, 30, 256, 256, 128, 0.3, 1.5, 1.0, 0, 0)

end
