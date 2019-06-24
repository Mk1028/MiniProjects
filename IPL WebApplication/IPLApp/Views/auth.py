from django import forms

class loginForm(forms.Form):
    username = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'class':'input','placeholder': 'Enter UserName'})
    )
    password = forms.CharField(
        required=True,
        widget=forms.PasswordInput(attrs={'placeholder': 'Enter Password'})
    )


class signUpForm(forms.Form):
    first_name = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'placeholder': 'Enter FirstName'})
    )
    last_name = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'placeholder': 'Enter LastName'})
    )
    username = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'placeholder': 'Enter UserName'})
    )
    password = forms.CharField(
        required=True,
        widget=forms.PasswordInput(attrs={'placeholder': 'Enter Password'})
    )
    # ProfilePhoto = forms.FileField(label='Upload File')
