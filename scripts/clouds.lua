--Clouds
--by: Explorer

function setup()
xs = 256;
ys = 256;
clouds = newTexture(xs, ys);
perlinNoise(clouds, 20, 20, 20, .6, 4, -1);

brightnessMult(clouds, 1.7);
clampBelow(clouds, 180, 100, 100, 250);
applyBlur(clouds, 0, 2, 1, 1, 1);

makeFinal(clouds);
end