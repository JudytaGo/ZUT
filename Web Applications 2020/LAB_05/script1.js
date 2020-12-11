let map = L.map('map').setView([53.430127, 14.564802], 18);
L.tileLayer.provider('Esri.WorldImagery').addTo(map);

Notification.requestPermission();

    document.getElementById("saveButton").addEventListener("click", function() {
            leafletImage(map, function (err, canvas) {
                // here we have the canvas
                let rasterMap = document.getElementById("rasterMap");
                let rasterContext = rasterMap.getContext("2d");

                rasterContext.drawImage(canvas, 0, 0, 300, 150);
            });
        });

        document.getElementById("getLocation").addEventListener("click", function(event) {
            if (! navigator.geolocation) {
                console.log("No geolocation.");
            }

            navigator.geolocation.getCurrentPosition(position => {
                console.log(position);
                let lat = position.coords.latitude;
                let lon = position.coords.longitude;

                map.setView([lat, lon]);
                let marker = L.marker([lat, lon]).addTo(map);
                marker.bindPopup("<strong>Hello!</strong><br>You're here!");
            }, positionError => {
                console.error(positionError);
            });
        });
        
    document.getElementById("getNewPuzzle").addEventListener("click",function(){
        leafletImage(map, function (err, canvas) {
            let rasterMap = document.getElementById("puzzleMap");
            let rasterContext = rasterMap.getContext("2d");
            rasterMap.width = canvas.width;
            rasterMap.height = canvas.height;
            let puzzles = [];
			let puzzle;
            let xOnePuzzle = rasterMap.width/4;
            let yOnePuzzle = rasterMap.height/4;
            for(let i = 0;i < 4;i++){
				for(let j=0;j<4;j++){
                	puzzle = {};
                	puzzle.x = xOnePuzzle*j;
                	puzzle.y = yOnePuzzle*i;
                	puzzles.push(puzzle);
				}
            }
            puzzles=shuffle(puzzles);
			
            for(let i = 0;i < 4;i++){
				for(let j = 0;j < 4;j++){
					puzzles[i*4+j].xPos=xOnePuzzle*j;
					puzzles[i*4+j].yPos=yOnePuzzle*i;
                	rasterContext.drawImage(canvas, puzzles[i*4+j].x, puzzles[i*4+j].y, xOnePuzzle, yOnePuzzle, xOnePuzzle*j, yOnePuzzle*i, xOnePuzzle, yOnePuzzle);
                	rasterContext.strokeRect(xOnePuzzle*j, yOnePuzzle*i, xOnePuzzle,yOnePuzzle);
				}
            }
            let mouse = {x:0,y:0};
            let currentPuzzle;
            document.getElementById("puzzleMap").onmousedown = function(e){
                mouse.x=e.layerX;
                mouse.y=e.layerY;
                for(i = 0;i < puzzles.length;i++){
                    puzzle = puzzles[i];
                    if(mouse.x < puzzle.xPos || mouse.x > (puzzle.xPos + xOnePuzzle) || mouse.y < puzzle.yPos || mouse.y > (puzzle.yPos + yOnePuzzle)){
                        currentPuzzle = null;
                    }
                    else{
                        currentPuzzle = puzzle;
                        break;
                    }
                }

                if(currentPuzzle != null) {
					
                    let puzzleUnder;
                    document.onmousemove = function(e){
                        puzzleUnder = null;
                        mouse.x=e.layerX;
                        mouse.y=e.layerY;
                        rasterContext.clearRect(0,0,canvas.width,canvas.height);
                        for(i = 0;i < puzzles.length;i++){
                            puzzle = puzzles[i];
                            if(puzzle == currentPuzzle){
                                continue;
                            }
                            rasterContext.drawImage(canvas, puzzle.x, puzzle.y, xOnePuzzle, yOnePuzzle, puzzle.xPos, puzzle.yPos, xOnePuzzle, yOnePuzzle);
                            rasterContext.strokeRect(puzzle.xPos, puzzle.yPos, xOnePuzzle,yOnePuzzle);
                            if(puzzleUnder == null){
                                if(!(mouse.x < puzzle.xPos || mouse.x > (puzzle.xPos + xOnePuzzle) || mouse.y < puzzle.yPos || mouse.y > (puzzle.yPos + yOnePuzzle))){
                                    puzzleUnder = puzzle;
                                    rasterContext.save();
                                    rasterContext.globalAlpha = .5;
									rasterContext.fillRect(puzzleUnder.xPos,puzzleUnder.yPos,xOnePuzzle, yOnePuzzle);
                                    rasterContext.restore();
                                }
                            }
                        }
                        rasterContext.save();
                        rasterContext.drawImage(canvas, currentPuzzle.x, currentPuzzle.y, xOnePuzzle, yOnePuzzle, mouse.x - (xOnePuzzle / 2), mouse.y - (yOnePuzzle / 2), xOnePuzzle, yOnePuzzle);
                        rasterContext.restore();
                        rasterContext.strokeRect( mouse.x - (xOnePuzzle / 2), mouse.y - (yOnePuzzle / 2), xOnePuzzle,yOnePuzzle);
                    };
                    document.onmouseup = function(e){
                        document.onmouseup = null;
                        document.onmousemove = null;
                        if(puzzleUnder != null){
                            let tmp = {xPos:currentPuzzle.xPos,yPos:currentPuzzle.yPos};
                            currentPuzzle.xPos = puzzleUnder.xPos;
                            currentPuzzle.yPos = puzzleUnder.yPos;
                            puzzleUnder.xPos = tmp.xPos;
                            puzzleUnder.yPos = tmp.yPos;
                        }
                        rasterContext.clearRect(0,0,canvas.width,canvas.height);
                        let win = true;
                        for(i = 0;i < puzzles.length;i++) {
                            puzzle = puzzles[i];
                            rasterContext.drawImage(canvas, puzzle.x, puzzle.y, xOnePuzzle, yOnePuzzle, puzzle.xPos, puzzle.yPos, xOnePuzzle, yOnePuzzle);
                            rasterContext.strokeRect(puzzle.xPos, puzzle.yPos, xOnePuzzle, yOnePuzzle);
                            if (puzzle.xPos != puzzle.x || puzzle.yPos != puzzle.y) {
                                win = false;
                            }
                        }
                        if(win){
                            rasterContext.drawImage(canvas,0,0);
                            setTimeout(Win(),1000);
                        }
                    };
                }};
        });
    });


	function shuffle(array) {
  		var currentIndex = array.length, temporaryValue, randomIndex;
  		while (0 !== currentIndex) {
    		randomIndex = Math.floor(Math.random() * currentIndex);
    		currentIndex -= 1;
    		temporaryValue = array[currentIndex];
    		array[currentIndex] = array[randomIndex];
    		array[randomIndex] = temporaryValue;
  		}
  	return array;
	}

    function Win(){       
        alert("Congratulations!"+ "\n");
        var notification = new Notification("Congratulations!");
    }
    