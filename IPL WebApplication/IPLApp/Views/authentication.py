from django.views import View
from django.shortcuts import render
from .auth import *
from django.contrib import messages
from django.contrib.auth import authenticate,login,logout
from django.urls import resolve
from django.http import HttpResponseRedirect
from django.contrib.auth.models import User

class Logout(View):
    def get(self,request):
        logout(request)
        return HttpResponseRedirect('/login/')

class LoginController(View):

    def get(self,request,*args,**kwargs):

        if(request.user.is_authenticated):
            return HttpResponseRedirect('/ipl/')

        return render(
            request,
            "login.html",
            context={'login':loginForm()}
        )

    def post(self,request):
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(request, username=username, password=password)
        if user is not None:
            login(request, user)
            return HttpResponseRedirect('/ipl/')
        else:
            messages.error(request,"Invalid Credentials")
            return HttpResponseRedirect('/login/')


class SignUpController(View):
    def get(self,request, *args, **kwargs):

        return render(
            request,
            "signup.html",
            context={'signUp': signUpForm()}
        )

    def post(self,request):
        form=signUpForm(request.POST)#,request.FILES)

        if form.is_valid():
            user=User.objects.create_user(**form.cleaned_data)
            # ,ProfilePhoto=request.FILES['ProfilePhoto'])
            user.save()

        user = authenticate(request, username=form.cleaned_data['username'], password=form.cleaned_data['password'])

        if user is not None:
            login(request, user)
            return HttpResponseRedirect('/login/')
        else:
            messages.error(request,"Invalid Credentials")
            return HttpResponseRedirect('/signup/')