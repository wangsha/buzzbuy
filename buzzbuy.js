
function setRecommend(e){
    var rate = e.id.substr(2, 1);
    document.getElementById('recommend').value = rate;
    var origin = '#319BD8';
    var color;
    //reset color
    document.getElementById('r_0').style.color = origin;
    document.getElementById('r_1').style.color = origin;
    document.getElementById('r_2').style.color = origin;

    switch (rate) {
        case '0':
            color = 'green';
            break;
        case '1':
            color = 'black';
            break;
        case '2':
            color = 'red';
            break;
    }
    document.getElementById(e.id).style.color = color;
}
