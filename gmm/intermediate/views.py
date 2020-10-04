from django.shortcuts import render
from django.http import HttpResponse

def index(request):
    return render(request, 'intermediate/index.html', {})
from django.shortcuts import render

# Create your views here.
