--by Barebones

function setup ()

shape = newTexture (256, 256)
land  = newTexture (256, 256)
water = newTexture (256, 256)

makeFinal (water)

perlinNoise (water, 0, 0, 100, 0.2, 3, -1)
tint (water, 0, 128, 192)

perlinNoise (shape, 0, 0, 30, 0.2, 3, -1)
quantize (shape, 2, 2, 2)

fractionalNoise (land, -1, 128, 128, 0, 255, 255, 0)
tint (land, 192, 128, 0)
overlay (land, shape, 1.0, 128, 128, 128)

applyBlur (land, 0, 1, 1, 1, 0)
light (land, land, 0, 128, 128, 64, 0.5, 1, 1.0, 0, 0)

overlay (water, land, 0.0, 0, 0, 0)

end
