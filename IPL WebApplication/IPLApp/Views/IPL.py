from django.views import View
from django.shortcuts import render
from django.urls import resolve
from django.http import HttpResponseRedirect
from django.contrib.auth.mixins import LoginRequiredMixin
from django.contrib.auth.models import User,Permission
from IPLApp.models import *
from django.db.models import Count,Sum

def Points(year):
    m = Matches.objects.values('team1', 'team2', 'result', 'winner').filter(season=year)

    teams = Matches.objects.values('team1').filter(season=year).distinct()

    points = dict()

    for i in teams:
        points[i['team1']] = 0

    for i in m:
        if (i['result'] == "no result"):
            points[i['team1']] += 1
            points[i['team2']] += 1
        else:
            points[i['winner']] += 1

    x = (sorted(points.items(), key=lambda kv: (kv[1], kv[0]), reverse=True))

    points = dict()
    for i in x:
        points[i[0]] = i[1]

    return points

def pointsTable(request,year):

    points=Points(year)

    return render(request,"PointsTable.html",{'points':points})

def SeasonMatches(request,year):
    m=Matches.objects.values('id','team1','team2','venue','toss_winner','toss_result','winner','player_of_match').filter(season=year)
    length = len(m)

    context = dict()
    context['matches'] = m
    context['year'] = year - 2008 + 1
    context['year1'] = year

    l=m.values().last()
    live=Deliveries.objects.values().filter(match_id=l['id'])
    inn1=live.filter(dl_applied=1).aggregate(wickets=Count('player_dismissed'))
    wides = live.filter(dl_applied=1, over=20).exclude(wide_runs=0).aggregate(wides=Count('wide_runs'))
    overs=live.values('over','ball').filter(dl_applied=1).last()

    if((inn1['wickets']==10) or (overs['over']==20 and overs['ball']==(6+wides['wides']))):
        inn1=live.filter(dl_applied=1).aggregate(total=Sum('total_runs'),wickets=Count('player_dismissed'))
        inn2=live.filter(dl_applied=2).aggregate(total=Sum('total_runs'),wickets=Count('player_dismissed'))
        wides = live.filter(dl_applied=2, over=20).exclude(wide_runs=0).aggregate(wides=Count('wide_runs'))
        overs = live.values('over', 'ball').filter(dl_applied=2).last()

        if((inn2['total'] > inn1['total']) or (inn2['wickets']==10) or (overs['over']==20 and overs['ball']==(6+wides['wides']))):
            context['matches'] = m
        else:
            m = Matches.objects.values('id', 'team1', 'team2', 'venue', 'toss_winner', 'toss_result', 'winner',
                                       'player_of_match').filter(season=year)[:length - 1]
            context['matches']=m
            context['live']=l
    else:
        m = Matches.objects.values('id', 'team1', 'team2', 'venue', 'toss_winner', 'toss_result', 'winner',
                                   'player_of_match').filter(season=year)[:length - 1]
        context['matches'] = m
        context['live']=l

    return render(request,"SeasonMatches.html",context)

class IPL(LoginRequiredMixin,View):
    login_url = '/login/'

    def get(self,request):

        latestYear=2019

        ids = list(Matches.objects.all().values_list('id', flat=True).filter(season=latestYear))

        top_batsmen = Deliveries.objects.filter(match__in=ids).values('batting_team', 'batsman').annotate(
            total=Sum('batsman_runs')).order_by('-total')[:1]

        top_bowler = Deliveries.objects.filter(match__in=ids).exclude(player_dismissed=None).values('bowling_team','bowler').annotate(total=Count('bowler')).order_by('-total')[:1]


        points = Points(latestYear)

        final = Matches.objects.values('id','team1','team2','venue','toss_winner','toss_result','winner','player_of_match').filter(season=latestYear).last()

        return render(request,
                      "HomePage.html",
                      {'points': points,
                       'final':final,
                       'season':latestYear-2008+1,
                       'year':latestYear,
                       'bat':top_batsmen[0],
                       'bowl':top_bowler[0],
                       'user': request.user
                      }
                     )

class SeasonList(View):

    def get(self, request):
        return render(request,"seasons.html")

def matchDetails(request,year,id):
    m=Matches.objects.values('team1','team2','venue','toss_winner','toss_result','winner','player_of_match').filter(season=year,id=1)
    match=Matches.objects.filter(season=year).count()

    d=Deliveries.objects.values('batting_team','bowling_team','over','ball','batsman','bowler','wide_runs','total_runs','player_dismissed').filter(match=id,dl_applied=1)

    team1=Deliveries.objects.values('batting_team').filter(match=id,dl_applied=1).first()
    team2=Deliveries.objects.values('batting_team').filter(match=id,dl_applied=2).first()


    top_batsmen_1 = Deliveries.objects.filter(match=id, dl_applied=1).values('batsman').annotate(
        total=Sum('batsman_runs')).order_by('-total')[:3]
    top_batsmen_2 = Deliveries.objects.filter(match=id, dl_applied=2).values('batsman').annotate(
        total=Sum('batsman_runs')).order_by('-total')[:3]

    top_bowler_1 = Deliveries.objects.filter(match=id, dl_applied=1).exclude(
        player_dismissed=None).values('bowler').annotate(total=Count('bowler')).order_by('-total')[:3]
    top_bowler_2 = Deliveries.objects.filter(match=id, dl_applied=2).exclude(
        player_dismissed=None).values('bowler').annotate(total=Count('bowler')).order_by('-total')[:3]

    return render(request,"MatchDetails.html",{'matches':m,
                                               'year':year,
                                               'id':id,
                                               'match':(id%(match+1)),
                                               'deliveries':d,
                                               'top_batsmen_1':top_batsmen_1,
                                               'top_batsmen_2':top_batsmen_2,
                                               'top_bowler_1':top_bowler_1,
                                               'top_bowler_2':top_bowler_2,
                                               'team1':team1,
                                               'team2':team2
                                               }
                  )

def liveDetails(request,year,id):
    m=Matches.objects.values('team1','team2','venue','toss_winner','toss_result','winner','player_of_match').filter(season=year,id=1)
    match=Matches.objects.filter(season=year).count()

    d=Deliveries.objects.values('batting_team','bowling_team','over','ball','batsman','bowler','wide_runs','total_runs','player_dismissed').filter(match=id,dl_applied=1)

    team1=Deliveries.objects.values('batting_team').filter(match=id,dl_applied=1).first()
    team2=Deliveries.objects.values('batting_team').filter(match=id,dl_applied=2).first()

    return render(request,"LiveDetails.html",{'matches':m,
                                               'year':year,
                                               'id':id,
                                               'match':(id%(match+1)),
                                               'deliveries':d,
                                               'team1':team1,
                                               'team2':team2
                                               }
                  )

def innings(request,year,id,innings):

    d=Deliveries.objects.values('batting_team','bowling_team','over','ball','batsman','bowler','wide_runs','total_runs','player_dismissed').filter(match=id,dl_applied=innings)

    match = Matches.objects.filter(season=year).count()

    return render(request, "Innings2.html", {
                                                 'deliveries': d,
                                                'match': (id % (match + 1)),
                                                'year': year,
                                                'id': id,
                                                 }
                  )

def teamPage(request,team):
    return render(request,"TeamPage.html",{'team':team})

def teamHomePage(request,team,year):
    m=Matches.objects.values('id','team1','team2','venue','toss_winner','toss_result','winner','player_of_match').filter(season=year,team1=team) | \
      Matches.objects.values('id','team1','team2','venue','toss_winner','toss_result','winner','player_of_match').filter(season=year,team2=team)


    final = Matches.objects.values('team1', 'team2','winner').filter(season=year).last()
    return render(request,"TeamHomePage.html",{'matches':m,'year':year-2008+1,'y':year,'final':final,'team':team})

def statistics(request,year):

    return render(request,"Statistics.html",{'year':year-2008+1,'yr':year})

def Filter(h1):
    h2 = []
    h3 = []
    for i in h1:
        if i['batsman'] not in h3:
            h2.append({'batsman': i['batsman'], 'batting_team': i['batting_team'], 'total': 1})
            h3.append(i['batsman'])
        else:
            for j in h2:
                if (j['batsman'] == i['batsman']):
                    j['total'] += 1

    for i in h2:
        i['team'] = i.pop('batting_team')
        i['name'] = i.pop('batsman')

    h2.sort(key=lambda x: x['total'],reverse=True)

    return h2

def stats(request,stat,year):

    ids = list(Matches.objects.all().values_list('id', flat=True).filter(season=year))

    context=dict()
    context['stat']=stat

    if(stat=="Runs"):
        top_batsmen = Deliveries.objects.filter(match__in=ids).values('batting_team', 'batsman').annotate(
            total=Sum('batsman_runs')).order_by('-total')[:25]

        for i in top_batsmen:
            i['team'] = i.pop('batting_team')
            i['name'] = i.pop('batsman')

        context['key']=top_batsmen

    elif(stat=="Wickets"):
        top_bowler = Deliveries.objects.filter(match__in=ids).exclude(player_dismissed=None).values('bowling_team',
            'bowler').annotate(total=Count('bowler')).order_by('-total')[:25]

        for i in top_bowler:
            i['team'] = i.pop('bowling_team')
            i['name'] = i.pop('bowler')

        context['key']=top_bowler

    elif(stat=="Catches"):
        catcher = Deliveries.objects.filter(match__in=ids, dismissal_kind='caught').exclude(fielder=None).values(
            'bowling_team','fielder').annotate(total=Count('fielder')).order_by('-total')

        for i in catcher:
            i['team'] = i.pop('bowling_team')
            i['name'] = i.pop('fielder')

        context['key']=catcher

    elif(stat=="Sixes"):
        sixes = Deliveries.objects.filter(match__in=ids, batsman_runs=6).values('batting_team', 'batsman').annotate(
            total=Count('batsman_runs')).order_by('-total')[:25]

        for i in sixes:
            i['team'] = i.pop('batting_team')
            i['name'] = i.pop('batsman')

        context['key']=sixes

    elif (stat == "Fours"):
        fours = Deliveries.objects.filter(match__in=ids, batsman_runs=4).values('batting_team', 'batsman').annotate(
            total=Count('batsman_runs')).order_by('-total')[:25]

        for i in fours:
            i['team'] = i.pop('batting_team')
            i['name'] = i.pop('batsman')

        context['key'] = fours

    elif (stat == "Hundreds"):
        h1 = []
        for i in ids:
            h = Deliveries.objects.filter(match=i).values('batting_team', 'batsman').annotate(
                total=Sum('batsman_runs')).filter(total__gte=100).order_by('-total')
            for j in h:
                if (j != []):
                    h1.append(j)

        h2=Filter(h1)

        context['key'] = h2

    elif (stat == "Fifties"):
        h1 = []
        for i in ids:
            h = Deliveries.objects.filter(match=i).values('batting_team', 'batsman').annotate(
                total=Sum('batsman_runs')).filter(total__gte=50).order_by('-total')
            for j in h:
                if (j != []):
                    h1.append(j)

        h2=Filter(h1)

        context['key'] = h2

    return render(request,"Stats.html",context)



