import pytest
from sim.fan_model import FanModel
def test_fan_converges():
 f=FanModel(); last=f.step(50,.1)
 for _ in range(20):
  now=f.step(50,.1); assert now>=last; last=now
 assert last==pytest.approx(1600,abs=1)
def test_bad_inputs():
 f=FanModel()
 with pytest.raises(ValueError): f.step(101,.1)
 with pytest.raises(ValueError): f.step(50,0)
