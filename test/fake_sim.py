import numpy as np
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def generate_imu_data():
    """IMU sensöründen gelen veriyi simüle eder."""
    accel = np.random.normal(0, 0.2, 3)  # m/s^2
    gyro = np.random.normal(0, 0.05, 3)  # rad/s
    mag = np.random.normal(0, 0.05, 3)  # µT
    return accel, gyro, mag

def generate_gps_data(lat, lon, alt):
    """GPS sensöründen gelen veriyi simüle eder."""
    lat_noise = np.random.normal(0, 0.00001)
    lon_noise = np.random.normal(0, 0.00001)
    alt_noise = np.random.normal(0, 0.5)  # metre cinsinden
    return lat + lat_noise, lon + lon_noise, alt + alt_noise

def simulate_sensors(duration=10, update_rate=10):
    """Belirli bir süre boyunca sensör verilerini simüle eder."""
    lat, lon, alt = 40.0, 29.0, 100.0  # Başlangıç GPS konumu
    dt = 1.0 / update_rate
    
    time_series = []
    altitudes = []
    accels = []
    gyros = []
    mags = []
    
    start_time = time.time()
    
    fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(10, 15))
    
    def update(frame):
        current_time = time.time() - start_time
        accel, gyro, mag = generate_imu_data()
        nonlocal lat, lon, alt
        lat, lon, alt = generate_gps_data(lat, lon, alt)
        
        data = f"IMU: Accel={accel.tolist()}, Gyro={gyro.tolist()}, Mag={mag.tolist()}\nGPS: Lat={lat:.6f}, Lon={lon:.6f}, Alt={alt:.2f}\n"
        print(data)
        
        time_series.append(current_time)
        altitudes.append(alt)
        accels.append(accel)
        gyros.append(gyro)
        mags.append(mag)
        
        if len(time_series) > 100:
            time_series.pop(0)
            altitudes.pop(0)
            accels.pop(0)
            gyros.pop(0)
            mags.pop(0)
        
        ax1.clear()
        ax2.clear()
        ax3.clear()
        ax4.clear()
        
        ax1.plot(time_series, altitudes, label="GPS Yükseklik")
        ax1.set_xlabel("Zaman (s)")
        ax1.set_ylabel("Yükseklik (m)")
        ax1.set_title("GPS Yükseklik Değişimi")
        ax1.legend()
        ax1.grid()
        
        ax2.plot(time_series, [a[0] for a in accels], label="Accel X")
        ax2.plot(time_series, [a[1] for a in accels], label="Accel Y")
        ax2.plot(time_series, [a[2] for a in accels], label="Accel Z")
        ax2.set_xlabel("Zaman (s)")
        ax2.set_ylabel("İvme (m/s^2)")
        ax2.set_title("IMU İvme Verileri")
        ax2.legend()
        ax2.grid()
        
        ax3.plot(time_series, [g[0] for g in gyros], label="Gyro X")
        ax3.plot(time_series, [g[1] for g in gyros], label="Gyro Y")
        ax3.plot(time_series, [g[2] for g in gyros], label="Gyro Z")
        ax3.set_xlabel("Zaman (s)")
        ax3.set_ylabel("Açısal Hız (rad/s)")
        ax3.set_title("IMU Gyro Verileri")
        ax3.legend()
        ax3.grid()
        
        ax4.plot(time_series, [m[0] for m in mags], label="Mag X")
        ax4.plot(time_series, [m[1] for m in mags], label="Mag Y")
        ax4.plot(time_series, [m[2] for m in mags], label="Mag Z")
        ax4.set_xlabel("Zaman (s)")
        ax4.set_ylabel("Manyetik Alan (µT)")
        ax4.set_title("IMU Mag Verileri")
        ax4.legend()
        ax4.grid()
        
        time.sleep(dt)
    
    ani = FuncAnimation(fig, update, frames=int(duration * update_rate), repeat=False)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    simulate_sensors()
