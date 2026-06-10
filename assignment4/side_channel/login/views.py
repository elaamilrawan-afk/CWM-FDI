import time
from secrets import compare_digest  # added
from django.shortcuts import render
from django.views.decorators.csrf import csrf_exempt

SECRET_PASSWORD = "519265"
SECRET_USERNAME = "admin"

# Delay per correct prefix character (seconds).
# Large enough to measure reliably on a local machine.
DELAY_PER_CHAR = 0.002


def _vulnerable_check(username: str, password: str) -> bool:
    """
    Intentionally vulnerable password check.
    Iterates character by character and sleeps on each match,
    leaking how many leading characters of the guess are correct.
    """
    if username != SECRET_USERNAME:
        return False

    for i, ch in enumerate(password):
        if i >= len(SECRET_PASSWORD) or ch != SECRET_PASSWORD[i]:
            return False
        time.sleep(DELAY_PER_CHAR)

    return len(password) == len(SECRET_PASSWORD)


def safe_check(username: str, password: str) -> bool:
    """
    stub for safe password check
    To be completed by you!
    """
    if username != SECRET_USERNAME:
        return False

    return compare_digest(password, SECRET_PASSWORD)






@csrf_exempt
def login_view(request):
    if request.method == "POST":
        username = request.POST.get("username", "")
        password = request.POST.get("password", "")

        t0 = time.perf_counter()
        success = safe_check(username, password) #has been replaced! # replace with safe_check as neede!
        elapsed_ms = (time.perf_counter() - t0) * 1000

        if success:
            return render(request, "login/login.html", {
                "success": True,
                "elapsed_ms": f"{elapsed_ms:.1f}",
            })
        return render(request, "login/login.html", {
            "error": "Invalid credentials.",
            "elapsed_ms": f"{elapsed_ms:.1f}",
            "guess": password,
        })

    return render(request, "login/login.html")
