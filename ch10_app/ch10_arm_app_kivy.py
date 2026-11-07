"""Robot arm control app in Python with Kivy (Android / desktop)
Talks to the arm of project 'Pick-and-Place Robotic Arm' through its HTTP API:
  /set?j=0&a=90   /home  /save  /play  /stop  /clear  /state
Desktop test:  pip install kivy   then   python ch10_arm_app_kivy.py
Android APK:   pip install buildozer ; buildozer init
               in buildozer.spec:  android.permissions = INTERNET
               buildozer android debug        (on Linux or WSL)
"""
from kivy.app import App
from kivy.clock import Clock
from kivy.network.urlrequest import UrlRequest
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.slider import Slider

ROBOT = "http://192.168.4.1"            # address of the ESP32 access point
JOINTS = ["Base", "Shoulder", "Elbow", "Gripper"]


class ArmApp(App):
    def build(self):
        self.pending = {}                # latest slider value per joint, sent 10 times a second
        root = BoxLayout(orientation="vertical", padding=20, spacing=10)
        for j, name in enumerate(JOINTS):
            row = BoxLayout(size_hint_y=None, height=70)
            label = Label(text=f"{name}: 90", size_hint_x=0.35)
            s = Slider(min=0, max=180, value=90, step=1)
            s.bind(value=lambda inst, v, j=j, lab=label, n=name: self.on_slider(j, int(v), lab, n))
            row.add_widget(label); row.add_widget(s); root.add_widget(row)
        buttons = BoxLayout(size_hint_y=None, height=70, spacing=6)
        for cmd in ["home", "save", "play", "stop", "clear"]:
            buttons.add_widget(Button(text=cmd.capitalize(), on_press=lambda b, c=cmd: self.send("/" + c)))
        root.add_widget(buttons)
        self.status = Label(text="...", size_hint_y=None, height=50)
        root.add_widget(self.status)
        Clock.schedule_interval(self.flush, 0.1)     # rate limit: do not flood the ESP32
        Clock.schedule_interval(self.poll, 1.0)      # read /state every second
        return root

    def on_slider(self, j, v, label, name):
        label.text = f"{name}: {v}"
        self.pending[j] = v

    def flush(self, dt):
        for j, v in list(self.pending.items()):
            self.send(f"/set?j={j}&a={v}")
        self.pending.clear()

    def send(self, path, on_ok=None):
        UrlRequest(ROBOT + path, on_success=on_ok, timeout=1,
                   on_error=lambda r, e: self.show("No connection to the robot"),
                   on_failure=lambda r, res: self.show("Robot answered with an error"))

    def poll(self, dt):
        self.send("/state", on_ok=lambda r, data: self.show(
            f"Saved poses: {data.get('poses')}  {'(playing)' if data.get('playing') else ''}"))

    def show(self, text):
        self.status.text = text


if __name__ == "__main__":
    ArmApp().run()
