import pytest

from sim.fan_model import FanModel


def test_fan_rpm_converges_toward_command():
    fan = FanModel()
    first = fan.step(50.0, 0.1)
    assert first > 0.0
    previous = first
    for _ in range(50):
        current = fan.step(50.0, 0.1)
        assert current >= previous
        previous = current
    assert current == pytest.approx(1600.0, abs=2.0)


def test_model_rejects_bad_inputs():
    fan = FanModel()
    with pytest.raises(ValueError):
        fan.step(101.0, 0.1)
    with pytest.raises(ValueError):
        fan.step(50.0, 0.0)
