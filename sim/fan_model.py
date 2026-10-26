"""Deterministic first-order fan model; illustrative, not calibrated hardware data."""
from dataclasses import dataclass
@dataclass
class FanModel:
    max_rpm: float = 3200.0
    time_constant_s: float = 0.8
    rpm: float = 0.0
    def step(self, duty_percent: float, dt_s: float) -> float:
        if not 0.0 <= duty_percent <= 100.0: raise ValueError("duty_percent must be in [0, 100]")
        if dt_s <= 0.0: raise ValueError("dt_s must be positive")
        if self.time_constant_s <= 0.0: raise ValueError("time_constant_s must be positive")
        target=self.max_rpm*duty_percent/100.0
        self.rpm+=(target-self.rpm)*min(dt_s/self.time_constant_s,1.0)
        return self.rpm
