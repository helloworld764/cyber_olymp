import sqlite3
from pathlib import Path
from flask import Flask, request, render_template_string

app = Flask(__name__)
DB = "shop.db"

TPL = '''
<h1>Магазин</h1>
<form method="get" action="/search">
  <input name="q" placeholder="поиск товара">
  <button>Найти</button>
</form>
<form method="post" action="/login">
  <input name="login" placeholder="логин">
  <input name="password" type="password" placeholder="пароль">
  <button>Войти</button>
</form>
<pre>{{ result }}</pre>
'''

def db():
    conn = sqlite3.connect(DB)
    conn.row_factory = sqlite3.Row
    return conn

@app.route("/")
def index():
    return render_template_string(TPL, result="")

@app.route("/login", methods=["POST"])
def login():
    username = request.form.get("login", "")
    password = request.form.get("password", "")
    with db() as conn:
        row = conn.execute(
            "SELECT username, role FROM users "
            "WHERE username = ? AND password_hash = ?",
            (username, password),
        ).fetchone()
    if row:
        return "Добро пожаловать, " + row["username"] + " (роль: " + row["role"] + ")"
    return "Неверные данные", 401

@app.route("/search")
def search():
    q = request.args.get("q", "")
    with db() as conn:
        rows = conn.execute(
            "SELECT title, price FROM items WHERE title LIKE ?",
            (f"%{q}%",),
        ).fetchall()
    text = "\n".join(f"{row['title']} - {row['price']}" for row in rows)
    return render_template_string(TPL, result=text)

if __name__ == "__main__":
    if not Path(DB).exists():
        with sqlite3.connect(DB) as conn:
            conn.executescript(Path(__file__).with_name("init_db.sql").read_text())
    app.run(host="0.0.0.0", port=5000, debug=False)
