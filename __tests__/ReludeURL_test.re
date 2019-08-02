open Jest;
open Expect;

describe("ReludeURL", () => {
  test("one", () =>
    expect(true) |> toEqual(true)
  );

  test("two", () =>
    expect(ReludeURL.x) |> toEqual("hi")
  );
});