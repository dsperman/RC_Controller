import tkinter as tk
from tkinter import ttk
import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

class MotorGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Motor Control GUI")
        self.root.geometry("900x500")

        self.serial_conn = None

        # ---- Left Frame (Controls) ----
        control_frame = tk.Frame(root)
        control_frame.pack(side=tk.LEFT, padx=10, pady=10, fill=tk.Y)

        # COM Port Dropdown
        tk.Label(control_frame, text="Select COM Port:").pack(anchor="w")

        self.combobox = ttk.Combobox(control_frame, state="readonly")
        self.combobox.pack(pady=5)

        self.refresh_ports()
        tk.Button(control_frame, text="Refresh Ports", command=self.refresh_ports).pack(pady=5)

        # ---- Connection Controls ----
        conn_frame = tk.Frame(control_frame)
        conn_frame.pack(pady=10)

        self.conn_button = tk.Button(conn_frame, text="Open Connection", command=self.toggle_connection, width=18)
        self.conn_button.pack(side=tk.LEFT)

        # Status indicator (circle)
        self.status_canvas = tk.Canvas(conn_frame, width=20, height=20, highlightthickness=0)
        self.status_canvas.pack(side=tk.LEFT, padx=10)

        self.status_circle = self.status_canvas.create_oval(2, 2, 18, 18, fill="grey")

        # ---- Command Buttons ----
        tk.Button(control_frame, text="Initialize Motor", width=20, command=self.initialize_motor).pack(pady=10)
        tk.Button(control_frame, text="Move to BDC", width=20, command=self.move_bdc).pack(pady=10)
        tk.Button(control_frame, text="Run Test", width=20, command=self.run_test).pack(pady=10)

        # ---- Right Frame (Plot) ----
        plot_frame = tk.Frame(root)
        plot_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

        self.fig, self.ax = plt.subplots(figsize=(5, 4))
        self.ax.set_title("Motor Data")
        self.ax.set_xlabel("X")
        self.ax.set_ylabel("Y")

        self.line, = self.ax.plot([], [])

        self.canvas = FigureCanvasTkAgg(self.fig, master=plot_frame)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        self.x_data = []
        self.y_data = []

    def refresh_ports(self):
        ports = serial.tools.list_ports.comports()
        port_list = [port.device for port in ports]
        self.combobox['values'] = port_list

        if port_list:
            self.combobox.current(0)

    # ---- Connection Handling ----
    def toggle_connection(self):
        if self.serial_conn and self.serial_conn.is_open:
            self.close_connection()
        else:
            self.open_connection()

    def open_connection(self):
        port = self.combobox.get()
        if not port:
            print("No COM port selected")
            return

        try:
            self.serial_conn = serial.Serial(port, 9600, timeout=1)
            print(f"Connected to {port}")
            self.update_connection_ui(True)
        except Exception as e:
            print(f"Connection failed: {e}")
            self.update_connection_ui(False)

    def close_connection(self):
        if self.serial_conn:
            self.serial_conn.close()
            print("Connection closed")
        self.update_connection_ui(False)

    def update_connection_ui(self, connected):
        if connected:
            self.conn_button.config(text="Close Connection")
            self.status_canvas.itemconfig(self.status_circle, fill="green")
        else:
            self.conn_button.config(text="Open Connection")
            self.status_canvas.itemconfig(self.status_circle, fill="grey")

    # ---- Command Functions ----
    def initialize_motor(self):
        print("Initialize Motor clicked")
        if self.serial_conn and self.serial_conn.is_open:
            self.serial_conn.write(b'INIT\n')
        else:
            print("Not connected")

    def move_bdc(self):
        print("Move to BDC clicked")
        if self.serial_conn and self.serial_conn.is_open:
            self.serial_conn.write(b'BDC\n')
        else:
            print("Not connected")

    def run_test(self):
        print("Run Test clicked")

        if not (self.serial_conn and self.serial_conn.is_open):
            print("Not connected")
            return

        for i in range(20):
            x = i
            y = i * i

            self.x_data.append(x)
            self.y_data.append(y)

            self.update_plot()

            self.root.update_idletasks()
            self.root.after(100)

            self.serial_conn.write(b'TEST\n')

    def update_plot(self):
        self.line.set_data(self.x_data, self.y_data)
        self.ax.relim()
        self.ax.autoscale_view()
        self.canvas.draw()


if __name__ == "__main__":
    root = tk.Tk()
    app = MotorGUI(root)
    root.mainloop()