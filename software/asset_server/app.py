from flask import Flask, request, redirect, send_from_directory, render_template_string
import os
import subprocess
from werkzeug.security import safe_join

app = Flask(__name__)

BASE_DIR = "/home/pi"

FOLDERS = ["VIDEO", "PATTERN", "EFFECT", "TEXT", "FONT"]

def list_dir(path):
    try:
        items = os.listdir(path)
    except:
        items = []

    folders = []
    files = []

    for i in items:
        full = os.path.join(path, i)
        if os.path.isdir(full):
            folders.append(i)
        else:
            files.append(i)

    return sorted(folders), sorted(files)


def safe_id(path):
    return path.replace("/", "_")

def safe_dir(base_dir, *child_path):
    safe_dest = safe_join(base_dir, os.path.join(*child_path))
    if safe_dest is None:
        app.logger.warning("Invalid Path: %r", child_path)
        return None
    else:
        return safe_dest

def render_folder(path, rel_path):
    folders, files = list_dir(path)

    uid = safe_id(rel_path)

    html = """
    <div class="folder-contents"><ul>
    """

    # FILES
    for f in files:
        file_rel = os.path.join(rel_path, f)

        html += """
        <div><li>
            {name}
            [<a href="/file/{file}" target="_blank">open</a>]
            [<a href="/delete/{file}">delete</a>]
        </li></div>
        """.format(name=f, file=file_rel)
    html += "</ul>"
    # SUBFOLDERS
    for d in folders:
        sub_path = os.path.join(path, d)
        sub_rel = os.path.join(rel_path, d)
        sub_id = safe_id(sub_rel)

        html += """
        <details>
            <summary>
                <b>{name}</b>
                <a href="#" onclick="document.getElementById('file_{id}').click()">[upload_files]</a>
                <a href="#" onclick="document.getElementById('dir_{id}').click()">[upload_folder]</a>
                <a href="#" onclick="newFolder_{id}()">[new_folder]</a>
                <a href="#" onclick="deleteFolder_{id}()">[delete_folder]</a>
            </summary>

            <form id="form_{id}" action="/upload/{path}" method="post" enctype="multipart/form-data" style="display:none;">
                <input type="file" name="file" id="file_{id}" multiple
                onchange="document.getElementById('form_{id}').submit()">
            </form>

            <form id="formdir_{id}" action="/upload/{path}" method="post" enctype="multipart/form-data" style="display:none;">
                <input type="file" name="file" id="dir_{id}" multiple webkitdirectory
                onchange="document.getElementById('formdir_{id}').submit()">
            </form>

            <form id="newfolder_{id}" action="/new_folder/{path}" method="post" style="display:none;">
                <input type="hidden" name="name" id="newfolder_input_{id}">
            </form>

            <form id="delfolder_{id}" action="/delete_folder/{path}" method="post" style="display:none;"></form>

            <script>
            function newFolder_{id}() {{
                var name = prompt("New folder name:");
                if (!name) return;

                document.getElementById("newfolder_input_{id}").value = name;
                document.getElementById("newfolder_{id}").submit();
            }}

            function deleteFolder_{id}() {{
                if (!confirm("Delete this folder and all contents?")) return;
                document.getElementById("delfolder_{id}").submit();
            }}
            </script>

            {children}
        </details>
        """.format(
            name=d,
            id=sub_id,
            path=sub_rel,
            children=render_folder(sub_path, sub_rel)
        )

    html += "</div>"

    return html

BASE_HTML = """
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <link rel="icon" href="/static/favicon.ico">

  <style>
    body {{
        font-family: sans-serif;
        margin: 20px;
        background: #ffffff;
        color: #111111;
    }}

    details {{
        margin-left: 12px;
        margin-top: 6px;
        padding: 6px;
        border: 1px solid #ddd;
        border-radius: 6px;
    }}

    summary {{
        cursor: pointer;
    }}

    a {{
        margin-left: 6px;
        color: #0645ad;
    }}

    @media (prefers-color-scheme: dark) {{
        body {{
            background: #121212;
            color: #e6e6e6;
        }}

        details {{
            border: 1px solid #333;
            background: #1e1e1e;
        }}

        a {{
            color: #8ab4f8;
        }}
    }}
  </style>
</head>

<body>
{content}
</body>
</html>
"""

@app.route("/")
def index():
    content = """
    <h1>RecurBOY Asset Server</h1>
    <hr>
    """

    html = BASE_HTML.format(content=content)

    for folder in FOLDERS:
        path = os.path.join(BASE_DIR, folder)
        os.makedirs(path, exist_ok=True)

        uid = safe_id(folder)

        html += """
        <details>
            <summary>
                <b>{folder}</b>
                <a href="#" onclick="document.getElementById('file_{uid}').click()">[upload_files]</a>
                <a href="#" onclick="document.getElementById('dir_{uid}').click()">[upload_folder]</a>
                <a href="#" onclick="newFolder_{uid}()">[new_folder]</a>
            </summary>

            <form id="form_{uid}" action="/upload/{folder}" method="post" enctype="multipart/form-data" style="display:none;">
                <input type="file" name="file" id="file_{uid}" multiple
                onchange="document.getElementById('form_{uid}').submit()">
            </form>

            <form id="formdir_{uid}" action="/upload/{folder}" method="post" enctype="multipart/form-data" style="display:none;">
                <input type="file" name="file" id="dir_{uid}" webkitdirectory
                onchange="document.getElementById('formdir_{uid}').submit()">
            </form>

            <form id="newfolder_{uid}" action="/new_folder/{folder}" method="post" style="display:none;">
                <input type="hidden" name="name" id="newfolder_input_{uid}">
            </form>

            <script>
            function newFolder_{uid}() {{
                var name = prompt("New folder name:");
                if (!name) return;

                document.getElementById("newfolder_input_{uid}").value = name;
                document.getElementById("newfolder_{uid}").submit();
            }}
            </script>
        """.format(folder=folder, uid=uid)

        html += render_folder(path, folder)

        html += "</details>"

    html += """
    <hr>
    <a href="/wifi">Configure WiFi Connection →</a>
    </body>
    </html>
    """

    return html

@app.route("/upload/<path:folder>", methods=["POST"])
def upload(folder):
    files = request.files.getlist("file")

    for file in files:
        if file and file.filename:
            safe_target = safe_dir(BASE_DIR, folder, file.filename)
            if safe_target is not None:
                os.makedirs(os.path.dirname(safe_target), exist_ok=True)
                file.save(safe_target)
            else:
                return redirect("/")

    return redirect("/")

@app.route("/file/<path:file_path>")
def file(file_path):
    full = os.path.join(BASE_DIR, file_path)
    folder = os.path.dirname(full)
    filename = os.path.basename(full)

    return send_from_directory(folder, filename)

@app.route("/delete/<path:file_path>")
def delete(file_path):
    safe_target = safe_dir(BASE_DIR, file_path)

    if safe_target is None:
        return redirect("/")
    if os.path.exists(safe_target):
        os.remove(safe_target)

    return redirect("/")

@app.route("/new_folder/<path:folder>", methods=["POST"])
def new_folder(folder):
    name = request.form.get("name", "").strip()

    if not name:
        return redirect("/")

    name = name.replace("/", "_")

    safe_target = safe_dir(BASE_DIR, folder, name)
    if safe_target is None:
        return redirect("/")
    os.makedirs(safe_target, exist_ok=True)

    return redirect("/")

@app.route("/delete_folder/<path:folder>", methods=["POST"])
def delete_folder(folder):
    safe_target = safe_join(BASE_DIR, folder)

    if safe_target is None:
        return redirect("/")

    if safe_target in {os.path.join(BASE_DIR, f) for f in FOLDERS}:
        return redirect("/")

    if os.path.exists(safe_target) and os.path.isdir(safe_target):
        import shutil
        shutil.rmtree(safe_target)

    return redirect("/")

WIFI_CONTENT = """
<h2>WiFi Settings</h2>
<p>Connection type: <b>{mode}</b></p>

{warning}

<form method="POST">

  <label>SSID</label><br>
  <input type="text" name="ssid" value="{ssid}" {disabled}><br><br>

  <label>Password</label><br>
  <input id="pw" type="password" name="password" value="{password}" {disabled}><br><br>

  <button type="button" onclick="togglePassword()">Show/Hide Password</button><br><br>

  <button type="submit" {disabled}>
    Save & Apply
  </button>
</form>
<hr>
<a href="/">← Back to asset server</a>

<script>
function togglePassword() {{
    var pw = document.getElementById("pw");
    pw.type = (pw.type === "password") ? "text" : "password";
}}
</script>
"""

def get_connection_type():
    try:
        client_ip = request.remote_addr

        route = subprocess.getoutput("ip route get " + client_ip)

        if "dev usb0" in route:
            return "usb"

        if "dev eth0" in route:
            return "ethernet"

        if "dev wlan0" in route:
            iw = subprocess.getoutput("iw dev wlan0 info")
            if "type AP" in iw:
                return "ap"
            return "wifi"

        return "offline"

    except:
        return "offline"



def get_ssid():
    return subprocess.getoutput("iwgetid -r").strip()

def apply_wifi(ssid, password):
    config = """country=00
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1

network={{
    ssid="{ssid}"
    psk="{password}"
}}
""".format(ssid=ssid, password=password)

    with open("/etc/wpa_supplicant/wpa_supplicant.conf", "w") as f:
        f.write(config)

    subprocess.call(["wpa_cli", "-i", "wlan0", "reconfigure"])

def load_wifi_config():
    ssid = ""
    password = ""

    try:
        with open("/etc/wpa_supplicant/wpa_supplicant.conf", "r") as f:
            lines = f.readlines()

        in_network = False

        for line in lines:
            line = line.strip()

            if line.startswith("network={"):
                in_network = True

            elif in_network and line.startswith("ssid="):
                ssid = line.split("=", 1)[1].replace('"', '')

            elif in_network and line.startswith("psk="):
                password = line.split("=", 1)[1].replace('"', '')

            elif line == "}":
                in_network = False

    except:
        pass

    return ssid, password

@app.route("/wifi", methods=["GET", "POST"])
def wifi():

    mode = get_connection_type()
    ssid, password = load_wifi_config()

    if mode == "wifi":
        warning = "<p style='color:red;'>⚠ Cannot change WiFi while connected via WiFi</p>"
        disabled = "disabled"
    else:
        warning = ""
        disabled = ""

    if request.method == "POST" and mode != "wifi":
        new_ssid = request.form["ssid"]
        new_pass = request.form["password"]
        apply_wifi(new_ssid, new_pass)
        return redirect("/wifi")

    content = WIFI_CONTENT.format(
        mode=mode,
        ssid=ssid,
        password=password,
        disabled=disabled,
        warning=warning
    )

    return BASE_HTML.format(content=content)
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
