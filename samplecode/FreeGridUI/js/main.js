$(function() {
	var csInterface = new CSInterface(); 
	
	// Update the color of the panel when the theme color of the product changed..
	updateThemeWithAppSkinInfo(csInterface.hostEnvironment.appSkinInfo);
	csInterface.addEventListener(CSInterface.THEME_COLOR_CHANGED_EVENT, onAppThemeColorChanged);
	
	// add all the event listeners we are going to need..
	csInterface.addEventListener('com.adobe.csxs.events.UpdatePanel', function(event) {
		var xmlData = $.parseXML(event.data);
		var $xml = $(xmlData);
		var gridType = $xml.find('gridtype').text();
		
		if (gridType == '0') {
			// moveTo is not enabled for this grid type
			$('#moveToRadio').attr('enabled', false);

		} else if (gridType == '1') {
			$('#moveToRadio').attr('enabled', true);
			var optionsStr = '<option>LeftPlane</option>';
			optionsStr += '<option>FloorPlane</option>';
			$('select[name="move"]').empty().append(optionsStr);
			
		} else {
			$('#moveToRadio').attr('enabled', true);
			var optionsStr = '<option>LeftPlane</option>';
			optionsStr += '<option>RightPlane</option>';
			optionsStr += '<option>FloorPlane</option>';
			$('select[name="move"]').empty().append(optionsStr);
		}
	});	
	
	csInterface.addEventListener('com.adobe.csxs.events.Error', function(event) {
		var xmlData = $.parseXML(event.data);
		var $xml = $(xmlData);
		var errorType = $xml.find('error').text();
		
		if (errorType == 'OutOfBounds'){
			alert('Sorry cannot move here as it will be out of bounds');
		}
	});	
	
	// restrict form to only one selected radio button..
	$('input[type=radio]').on('click',function(e) {
		$('input[type=radio]').prop('checked','');
		$(this).prop('checked','checked');
		e.preventDefault();
	});
	
	// disable/enable form elements when radio button is checked..
	$('input[type=radio]').on('click',function(e) {
		var selectedRadio = $('#settingsForm input[type="radio"]:checked').val();
		switch (selectedRadio) {
		case 'translate':
			$('#offsetX').attr('disabled', false);
			$('#offsetY').attr('disabled', false);
			$('#scale').attr('disabled', true);
			$('#rotate').attr('disabled', true);
			$('#move').attr('disabled', true);
			break;
		case 'scale':
			$('#offsetX').attr('disabled', true);
			$('#offsetY').attr('disabled', true);
			$('#scale').attr('disabled', false);
			$('#rotate').attr('disabled', true);
			$('#move').attr('disabled', true);
			break;
		case 'rotate':
			$('#offsetX').attr('disabled', true);
			$('#offsetY').attr('disabled', true);
			$('#scale').attr('disabled', true);
			$('#rotate').attr('disabled', false);
			$('#move').attr('disabled', true);
			break;
		case 'moveTo':
			$('#offsetX').attr('disabled', true);
			$('#offsetY').attr('disabled', true);
			$('#scale').attr('disabled', true);
			$('#rotate').attr('disabled', true);
			$('#move').attr('disabled', false);
			break;
		}
	});
	
	// form submit handler..
	$('#settingsForm').submit(function( event ) {	
		
		event.preventDefault();
		
		// check that a document is open (using extendscript)..
		evalScript('app.documents.length', function(result) {
			if (result == 0) {
				alert('An open document is required');
			} else {
				handleFormSubmit();
			}
		});
	});
	
	// let the FreeGrid plug-in know that we are ready..
	var panelReadyEvent = new CSEvent('com.adobe.csxs.events.PanelReady', 'APPLICATION', 'ILST', 'FreeGridUI');
	csInterface.dispatchEvent(panelReadyEvent);
});

function handleFormSubmit() {
	var offsetX = $('#offsetX').val();
	var offsetY = $('#offsetY').val();  
	var rotate = $('#rotate').val();
	var move = $('#move option:selected').text();
	var scale = $('#scale').val();

	// which radio the user selected determines the xml we send to the FreeGrid plug-in..
	var data = '';
	var selectedRadio = $('#settingsForm input[type="radio"]:checked').val();
	switch (selectedRadio) {
	case 'translate':
		data = '<payload><operation>0</operation><translate><v>'+offsetX+'</v><h>'+offsetX+'</h></translate></payload>';
		break;
	case 'scale':
		data = '<payload><operation>1</operation><scale><size>'+scale+'</size></scale></payload>';
		break;
	case 'rotate':
		data = '<payload><operation>2</operation><rotate><angle>'+rotate+'</angle></rotate></payload>';
		break;
	case 'moveTo':
		data = '<payload><operation>3</operation><moveto><plane>'+move+'</plane></moveto></payload>';
		break;
	}

	if (data != '') {
		var okEvent = new CSEvent('com.adobe.csxs.events.OkClicked', 'APPLICATION', 'ILST', 'FreeGridUI');
		okEvent.data = data;
		new CSInterface().dispatchEvent(okEvent);
	} else {
		// user has not selected a radio..
		alert('Must select a radio button');
	}
}

/**
* Update the theme with the AppSkinInfo retrieved from the host product.
*/
function updateThemeWithAppSkinInfo(appSkinInfo) {
	//Update the background color of the panel
	var panelBackgroundColor = appSkinInfo.panelBackgroundColor.color;
	document.body.bgColor = toHex(panelBackgroundColor);
        
	var styleId = 'ppstyle';
	addRule(styleId, '.default', 'font-size:' + appSkinInfo.baseFontSize + 'px' + '; color:' + reverseColor(panelBackgroundColor) + '; background-color:' + toHex(panelBackgroundColor, 20));
	addRule(styleId, 'button', 'border-color: ' + toHex(panelBackgroundColor, -50));
}

function addRule(stylesheetId, selector, rule) {
	var stylesheet = document.getElementById(stylesheetId);
    
	if (stylesheet) {
		stylesheet = stylesheet.sheet;
		if( stylesheet.addRule ){
			stylesheet.addRule(selector, rule);
		} else if( stylesheet.insertRule ) {
			stylesheet.insertRule(selector + ' { ' + rule + ' }', stylesheet.cssRules.length);
		}
	}
}

function reverseColor(color, delta) {
	return toHex({red:Math.abs(255-color.red), green:Math.abs(255-color.green), blue:Math.abs(255-color.blue)}, delta);
}

/**
* Convert the Color object to string in hexadecimal format;
*/
function toHex(color, delta) {
	function computeValue(value, delta) {
		var computedValue = !isNaN(delta) ? value + delta : value;
		if (computedValue < 0) {
			computedValue = 0;
		} else if (computedValue > 255) {
			computedValue = 255;
		}

		computedValue = computedValue.toString(16);
		return computedValue.length == 1 ? '0' + computedValue : computedValue;
	}

	var hex = '';
	if (color) {
		with (color) {
			hex = computeValue(red, delta) + computeValue(green, delta) + computeValue(blue, delta);
		};
	}
	return '#' + hex;
}

function onAppThemeColorChanged(event) {
	// Should get a latest HostEnvironment object from application.
	var skinInfo = JSON.parse(window.__adobe_cep__.getHostEnvironment()).appSkinInfo;
	// Gets the style information such as color info from the skinInfo, 
	// and redraw all UI controls of your extension according to the style info.
	updateThemeWithAppSkinInfo(skinInfo);
} 

function evalScript(script, callback) {
	new CSInterface().evalScript(script, callback);
}