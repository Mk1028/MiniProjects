from django.urls import path
from django.conf.urls import url
from .Views import *
from .Views.authentication import *

urlpatterns=[

    # path('api/login/', login),
    #
    path('', LoginController.as_view(), name="loginPage"),
    path('login/', LoginController.as_view(), name="login"),
    path('logout/', Logout.as_view(), name="logout"),
    path('signup/', SignUpController.as_view(), name="signUp"),

    path('ipl/', IPL.as_view(), name="seasonList"),
    path('seasons/',SeasonList.as_view(),name="seasonList"),
    path('stats/<int:year>/', statistics, name="statistics"),
    path('stats/<str:stat>/<int:year>/', stats, name="stats"),
    path('seasons/<int:year>/', SeasonMatches, name="season"),
    path('seasons/<str:team>/',teamPage, name="teamPage"),
    path('seasons/points_table/<int:year>/', pointsTable, name="season"),
    path('seasons/<int:year>/LIVE/<int:id>/', liveDetails, name="matchDetails"),
    path('seasons/<int:year>/<int:id>/', matchDetails, name="matchDetails"),
    path('seasons/<str:team>/<int:year>/', teamHomePage, name="teamHomePage"),
    path('seasons/<int:year>/<int:id>/<int:innings>/',innings, name="Innings2"),

]