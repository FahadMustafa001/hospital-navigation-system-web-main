const API_BASE = "http://127.0.0.1:8081";

const loginBtn = document.getElementById("loginBtn");
const loginMessage = document.getElementById("loginMessage");

async function doLogin() {
  const username = document.getElementById("username").value.trim();
  const password = document.getElementById("password").value.trim();

  loginMessage.classList.remove("success");
  loginMessage.classList.remove("error");

  if (!username || !password) {
    loginMessage.textContent = "Username and password are required.";
    loginMessage.classList.add("error");
    return;
  }

  try {
    const params = new URLSearchParams();
    params.set("user", username);
    params.set("pass", password);
    const resp = await fetch(`${API_BASE}/api/login`, {
      method: "POST",
      headers: {
        "Content-Type": "application/x-www-form-urlencoded",
      },
      body: params.toString(),
    });

    const data = await resp.json();
    if (data && data.success === true) {
      loginMessage.textContent = "Login successful!";
      loginMessage.classList.add("success");
      window.location.href = "../Navigation/index.html";
      return;
    }

    loginMessage.textContent = "Invalid username or password.";
    loginMessage.classList.add("error");
  } catch (e) {
    loginMessage.textContent = "Backend not reachable. Start the C++ server on port 8081.";
    loginMessage.classList.add("error");
  }
}

loginBtn.addEventListener("click", doLogin);

document.addEventListener("keydown", (e) => {
  if (e.key === "Enter") doLogin();
});
