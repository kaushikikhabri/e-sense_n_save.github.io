function handleLogin(event) {
  event.preventDefault(); // Prevent form submission

  var username = document.getElementById('username').value;
  var password = document.getElementById('password').value;

  if (username === 'admin' && password === 'password') {
    alert('Login successful!');
    window.location.href = 'index.html';
  } else {
    alert('Invalid username or password!');
  }
}

document.getElementById('login-form').addEventListener('submit', handleLogin);
