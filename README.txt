The original Instant Django was created by a different author, and downloaded from instantdjango.com
Since then, the domain no longer exists, and original installer is no longer available.
Furthermore, it only contained Django 1.2.4, and means to easily upgrade it(no setuptools/pip).

In order to keep this great dream alive, Kevin Veroneau the author of PythonDiary.com is releasing
an updated version for all to enjoy!  This is by far the easiest way to get up and running with Django.

When you run "start.bat", it will launch a command window with special variables, making it a sandbox.
The PATH variable is completely overwritten with a PATH that adds "python" and "django-admin" to it.
Thus, exactly like in a *NIX system, a Windows user, such as yourself can enjoy launching "python"
and "django-admin" with ease.  Here is a simple example to get you started:

C> django-admin startproject testsite
C> cd testsite
C> python manage.py runserver

There you have it, the basic Django Hello World site is up and running in seconds of downloading!

Enjoy!
