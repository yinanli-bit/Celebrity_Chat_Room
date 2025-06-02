import os
import sys
from http import HTTPStatus
from pathlib import Path
from dashscope import Application

DATA_DIR = Path("D:/codes/TCproject_0_2/data")
SESSION_FILE = DATA_DIR / "session_id.txt"
INPUT_FILE = DATA_DIR / "input.txt"
RESPONSE_FILE = DATA_DIR / "response.txt"
# ✅ 确保目录存在
DATA_DIR.mkdir(parents=True, exist_ok=True)
print(f"Python脚本真实路径: {__file__}")
print(f"输入文件绝对路径: {INPUT_FILE.absolute()}")
print(f"当前进程权限: {'可写' if os.access("D:/codes/TCproject_0_2/data", os.W_OK) else '不可写'}")
class ChatSession:
    def __init__(self):
        self.session_file = SESSION_FILE
        self._ensure_directory()
        self.session_id = self._load_session()

    def _ensure_directory(self):
        """确保文件目录存在"""
        os.makedirs(self.session_file.parent, exist_ok=True)

    def _load_session(self):
        try:
            if self.session_file.exists():
                with open(self.session_file, "r", encoding="utf-8") as f:
                    content = f.read().strip()
                    print(f"load session ID from file: {content}")
                    return content
            return None
        except Exception as e:
            print(f"load session ID fail: {str(e)}")
            return None

    def save_session(self, session_id):
        try:
            with open(self.session_file, "w", encoding="utf-8") as f:
                f.write(session_id)
            print(f"session ID saved: {session_id}")
        except Exception as e:
            print(f"save ID fail: {str(e)}")

    # def clear_session(self):
    #     try:
    #         os.remove(SESSION_FILE)
    #         self.session_id = None
    #     except Exception as e:
    #         print(f"clean session fail: {e}")

def call_dashscope(prompt, chat_session):
    params = {
        "api_key": os.getenv("DASHSCOPE_API_KEY"),
        "app_id": '717b9c6030a24077862bef16cb644675',
        "prompt": prompt,
        "session_id": chat_session.session_id
    }

    print(f"call params: {params}")

    response = Application.call(**params)

    if response.status_code == HTTPStatus.OK:
        new_session_id = response.output.session_id
        print(f"new session ID: {new_session_id}")
        if new_session_id and new_session_id != chat_session.session_id:
            chat_session.save_session(new_session_id)
        return response.output.text
    else:
        print(f"API wrong: {response.code} - {response.message}")
        return "request fail"


def process_chat():
    chat_session = ChatSession()

    try:
        # ✅ 确保文件存在
        if not INPUT_FILE.exists():
            return "no input_file"

        with open(INPUT_FILE, "r", encoding="utf-8") as f:
            prompt = f.read().strip()

        if prompt == "/reset":
            chat_session.clear_session()
            return "session clean"

        if not prompt:
            return "请输入有效内容"

        result = call_dashscope(prompt, chat_session)
        return result or "无有效响应"

    except Exception as e:
        print(f"处理流程异常: {str(e)}")
        return "系统错误"

if __name__ == '__main__':
    try:
        print(f"content: {os.getcwd()}")
        print(f"path: {INPUT_FILE}")

        if not INPUT_FILE.exists():
            print("wrong: no input.txt")
            with open(RESPONSE_FILE, "w", encoding="utf-8") as f:
                f.write("input missing")
            sys.exit(1)

        # ✅ 强制设置UTF-8编码
        sys.stdout.reconfigure(encoding='utf-8')
        sys.stderr.reconfigure(encoding='utf-8')

        response = process_chat()
        print(f"final response: {response}")

        # ✅ 确保响应文件写入
        with open(RESPONSE_FILE, "w", encoding="utf-8") as f:
            f.write(response)

    except Exception as e:
        print(f"主程序异常: {str(e)}")
        with open(RESPONSE_FILE, "w", encoding="utf-8") as f:
            f.write("系统内部错误")