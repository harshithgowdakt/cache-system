$("#searchForm").submit(function (event) {
    event.preventDefault();
    let $form = $(this),
        id = $form.find("input[name='ID']").val(),
        name = $form.find("input[name='Name']").val(),
        data = JSON.stringify({ id: id, name: name }),
        url = $form.attr("action");

    $.ajax({
        url: url,
        type: "POST",
        data: data,
        contentType: "application/json",
        success: function (data) {
            alert(data);
        },
        error: function (error) {
            alert(error.responseText);
        }
    });
});