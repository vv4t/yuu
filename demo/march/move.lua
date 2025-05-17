
function update(mx, my, forward, left, back, right)
  yaw = (mx * 2.0 - 1.0) * 4.0
  pitch = (my * 2.0 - 1.0) * 4.0
  
  c = math.cos(yaw)
  s = math.sin(yaw)
  
  dx = (right - left) * 0.1
  dz = (forward - back) * 0.1
  
  view_pos.x = view_pos.x + dx * c + dz * s
  view_pos.z = view_pos.z + dx * -s + dz * c
end
