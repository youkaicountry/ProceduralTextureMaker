--Concrete
--by Cryoburner

function setup()

--texture
terrain = newTexture(256, 256);
terrain2 = newTexture(256, 256);
fractionalNoise(terrain2, 0, os.time(), 0, 0, 0, 64, 64, 64);
copyTexture(terrain2, terrain);
for i = 1, 4, 1 do
 applyScale(terrain, 2.0, 2.0, 0, 0, 0)
 applyBlur(terrain, 0, 1, 1, 1, 1)
 add(terrain, terrain2, 2)
end;

--bumpmap
copyTexture(terrain2, terrain);
tint(terrain2, 224, 224, 224);
applyBlur(terrain2, 0, 8, 1, 1, 1);
quantize(terrain2, 2, 2, 2);
applyBlur(terrain2, 0, 5, 1, 1, 1);
add(terrain2, terrain, 2);

--combine
tint(terrain, 320, 310, 300);
light(terrain, terrain2, 8.0, 128, 128, 96, 0.0, 1.0, 1.0, 0.0, 0);
makeFinal(terrain);

end;