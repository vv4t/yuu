
a = 0
b = 0

size = 0.1

function update(x, y, up, left, down, right, shrink, grow)
  a = a + (right - left) * 0.01
  b = b + (up - down) * 0.01
  size = size + (grow - shrink) * 0.01
  
  pos.x = x + a
  pos.y = y + b
end
