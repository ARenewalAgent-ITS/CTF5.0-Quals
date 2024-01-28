from flask import Flask, request, Response, jsonify, redirect, url_for, render_template_string, abort
from util import is_from_localhost, proxy_req
import random, os

app = Flask(__name__)

blacklist = ["debug", "args", "headers", "cookies", "environ", "values", "query", "form", "os", "system", "popen", "subprocess", "globals", "locals", "self", "lipsum", "cycler", "joiner", "namespace", "init", "join", "module", "config", "builtins", "import", "application", "getitem", "read", "getitem", "mro", "'", '"', "_", "{{", " ", "[", "]"]

def sanitize_input(name):
    for word in blacklist:
        if word in name:
            return False
    return name

@app.route('/', methods=['GET'])
def proxy():
    url = request.args.get('url')

    list_endpoints = [
        '/about/',
        '/portfolio/',
    ]

    endpoint = random.choice(list_endpoints)

    if not url:
        return redirect(url_for('.proxy', url=endpoint))
    
    target_url = "http://daffa.info" + url

    if target_url.startswith("http://daffa.info") and any(target_url.endswith(endpoint) for endpoint in list_endpoints):
        response, headers = proxy_req(target_url)

        return Response(response.content, response.status_code, headers.items())
    else:
        abort(403)

@app.route('/environment', methods=['GET', 'POST'])
@is_from_localhost
def dev_environment():
    admin = "daffainfo"
    css_url = url_for('static', filename='css/main.css')
    image_url = url_for('static', filename='images/dewaweb.png')

    if request.args.get('admin') is not None:
        admin = request.args.get('admin')
        admin = sanitize_input(admin)

    if not admin:
        abort(403)

    template = '''<!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Admin Notes Preview</title>
            <link rel="stylesheet" href="{}">
        </head>
        <body>
            <h1>NOTES!! ONLY ADMIN CAN ACCESS THIS AREA!</h1>
            <form action="" method="GET">
                <label for="admin">Admin:</label>
                <input type="text" id="admin" name="admin" required>
                <br>
                <input type="submit" value="Preview!">
            </form>
            <p>Admin: {}<span id="adminName"></span></p>
            <br>
            <h3>Powered By</h3>
            <a href="https://www.dewaweb.com/">
                <img src="{}" alt="dewaweb">
            </a>
        </body>
        </html>'''.format(css_url, admin, image_url)
    return render_template_string(template)

app.run(host='0.0.0.0', port=1337)