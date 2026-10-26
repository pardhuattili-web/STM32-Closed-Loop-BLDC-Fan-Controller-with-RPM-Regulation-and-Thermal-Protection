import pytest

from sim.fan_model import FanModel


def test_fan_rpm_moves_toward_command():
    fan = FanModel()
    first = fan.step(50.0, 0.1)
    assert first > 0.0
    previous = first
    for _ in range(20):
        current = fan.step(50.0, 0.1)
        assert current >= previous
        previous = current
    assert current == pytest.approx(1600.0, abs=1.0)


def test_fan_model_rejects_invalid_input():
    fan = FanModel()
    with pytest.raises(ValueError):
        fan.step(101.0, 0.1)
    with pytest.raises(ValueError):
        fan.step(50.0, 0.0)
