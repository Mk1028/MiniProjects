from django.db import models

# Create your models here.
class Matches(models.Model):
    id=models.IntegerField(primary_key=True)
    season=models.IntegerField()
    city = models.CharField(max_length=128,null=True)
    date=models.DateField(null=True, blank=True)
    team1 = models.CharField(max_length=128,null=True)
    team2 = models.CharField(max_length=128,null=True)
    toss_winner = models.CharField(max_length=128,null=True)
    toss_result = models.CharField(max_length=128,null=True)
    result = models.CharField(max_length=128,null=True)
    dl_applied=models.IntegerField()
    winner = models.CharField(max_length=128,null=True)
    win_by_runs=models.IntegerField()
    win_by_wickets=models.IntegerField()
    player_of_match = models.CharField(max_length=128,null=True)
    venue = models.CharField(max_length=128,null=True)
    umpire1 = models.CharField(max_length=128,null=True)
    umpire2 = models.CharField(max_length=128,null=True)


    def __str__(self):
        return str(self.id)


class Deliveries(models.Model):
    match=models.ForeignKey(Matches,on_delete=models.CASCADE)

    dl_applied=models.IntegerField()
    batting_team = models.CharField(max_length=128,null=True)
    bowling_team = models.CharField(max_length=128,null=True)
    over=models.IntegerField()
    ball=models.IntegerField()
    batsman = models.CharField(max_length=128,null=True)
    non_striker = models.CharField(max_length=128,null=True)
    bowler = models.CharField(max_length=128,null=True)
    is_super_over=models.IntegerField()
    wide_runs=models.IntegerField()
    bye_runs=models.IntegerField()
    legbye_runs=models.IntegerField()
    noball_runs=models.IntegerField()
    penalty_runs=models.IntegerField()
    batsman_runs=models.IntegerField()
    extra_runs=models.IntegerField()
    total_runs=models.IntegerField()
    player_dismissed = models.CharField(max_length=128,null=True)
    dismissal_kind = models.CharField(max_length=128,null=True)
    fielder = models.CharField(max_length=128,null=True)


    def __str__(self):
        return str(self.match)