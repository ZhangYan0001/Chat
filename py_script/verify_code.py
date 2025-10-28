import smtplib
import random
import string
import time
from flask import Flask, request, jsonify, render_template_string
from flask_cors import CORS
from email.mime.text import MIMEText
from email.header import Header
import signal
import os

app = Flask(__name__)
CORS(app)

# 内存存储验证码: { "email": (code, expire_time) }
codes = {}


# 邮件配置py
SMTP_SERVER = "smtp.vip.163.com"  # SMTP服务器地址
SMTP_PORT = 465  # TLS端口 (常用: 587/465)
SMTP_USER = "yanzhang.hist@vip.163.com"
SMTP_PASS = "ULTBf32tkP99jCpT"


# 生成随机验证码
def generate_code(length=6):
  return "".join(random.choices(string.digits, k=length))


# 发送邮件
def send_email(to_email, code):
  subject = "您的验证码"
  content = f"您的验证码是 {code} ，有效期5分钟。"
  msg = MIMEText(content, "plain", "utf-8")
  msg["From"] = Header(SMTP_USER)
  msg["To"] = Header(to_email)
  msg["Subject"] = Header(subject, "utf-8")

  try:
    server = smtplib.SMTP_SSL(SMTP_SERVER, SMTP_PORT)
    server.login(SMTP_USER, SMTP_PASS)
    print("smtp login success")
    server.sendmail(SMTP_USER, [to_email], msg.as_string())
    server.quit()
    return True
  except Exception as e:
    print("发送失败:", e)
    return False


# 请求验证码接口
@app.route("/get_code", methods=["POST"])
def get_code():
  email = request.json.get("email")
  if not email:
    return jsonify({"error": "缺少邮箱"}), 400

  code = generate_code()
  expire_time = time.time() + 300  # 5分钟有效
  codes[email] = (code, expire_time)
  print("the code is ", code)
  if send_email(email, code):
    return jsonify({"message": "验证码已发送", "email": email})
  else:
    return jsonify({"error": "邮件发送失败", "email": email}), 500


# 校验验证码接口
@app.route("/verify_code", methods=["POST"])
def verify_code():
  email = request.json.get("email")
  code = request.json.get("code")

  if not email or not code:
    return jsonify({"error": "缺少参数"}), 400

  if email not in codes:
    return jsonify({"error": "请先获取验证码"}), 400

  saved_code, expire_time = codes[email]
  if time.time() > expire_time:
    return jsonify({"error": 3, "email": email}), 400

  if code == saved_code:
    return jsonify({"message": "验证成功", "email": email})
  else:
    return jsonify({"error": 3, "email": email})


# test code
@app.route("/")
def hello():
  return render_template_string("hello Qt")


@app.route("/shutdown")
def shutdown():
    func = request.environ.get("werkzeug.server.shutdown")
    if func:
        func()
    else:
        os.kill(os.getpid(), signal.SIGTERM)  # 兜底
    return "Server shutting down..."


if __name__ == "__main__":
  app.run(debug=True, port=5000)
