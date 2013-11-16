<script>

    var symbols = "";
    $(document).ready(function(){
    	
        $("#quotes th").hover(function() {
    	    $( this ).css("color", "#1240AB");
    	    $( this ).css("background-color", "#4671D5");
        }, function() {
    	    $( this ).css("color", "#4671D5");
    	    $( this ).css("background-color", "#1240AB");
        });
    	
    	$("#symbols").focus();
        $("#quotes").tablesorter();
        $('#symbols').keydown(function (e){
    		if(e.keyCode == 13){
    		    var newSymbols = $("#symbols").val().split(',');
    		    for (var i = 0; i < newSymbols.length; i++){
                    if (symbols.indexOf(newSymbols[i].toLowerCase()) < 0){
                        if (symbols.length > 0){
                            symbols = symbols +','+ newSymbols[i].toLowerCase();
                        }else{
                            symbols = newSymbols[i];
                        }
                    }
                }
                $("#symbols").val("");
    	    }
        });
        
    	$("#addSymbols").click(function(){
    	    var newSymbols = $("#symbols").val().split(',');
    	    for (var i = 0; i < newSymbols.length; i++){
                if (symbols.indexOf(newSymbols[i].toLowerCase()) < 0){
                    if (symbols.length > 0){
                        symbols = symbols +','+ newSymbols[i].toLowerCase();
                    }else{
                        symbols = newSymbols[i];
                    }
                }
            }
            $("#symbols").val("");
        });
        
        jQuery.fn.timer = function() {
            var sorting = $("#quotes").get(0).config.sortList;
            if (symbols.length > 0){
              $.getJSON( "/cgi-bin/sst.py?s=" + symbols, function(json) {
                $('#stocks').empty();
            	    for(var i=0; i < json.length; i++){
                        if (parseFloat(json[i].last, 10) !== 0.0){
                            $('#stocks').append('<tr><td>'+json[i].name+'</td><td>'+json[i].last+'</td><td>'+json[i].change+'</td><td>'+json[i].open+'</td><td>'+json[i].close+'</td></tr>');
                            if (parseFloat(json[i].change, 10) < 0.0){
                                $('#quotes tr:last').css("color", "rgb(255, 0, 0)");
                            }else if (parseFloat(json[i].change, 10) > 0.0){
                                $('#quotes tr:last').css("color", "rgb(0, 255, 0)");
                            }else{
                                $('#quotes tr:last').css("color", "rgb(255, 255, 0)");
                            }
                        }
                        $("#quotes").trigger("update");
            	    }
                });
              }
            }	
	window.setInterval(function() {
		$("#quotes").timer();
	}, 1000);
	
});
</script>
